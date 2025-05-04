#include "ImguiExample.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>

#include <memory.h>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	using namespace BoulderLeaf::Graphics::DX12;

	// Config for example app
	static const int APP_NUM_FRAMES_IN_FLIGHT = 2;// what is this?
	static const int APP_NUM_BACK_BUFFERS = SwapChainBufferCount;
	static const int APP_SRV_HEAP_SIZE = sSrvHeapSize;

	struct FrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		UINT64 FenceValue;
	};

	// Simple free list based allocator
	struct ExampleDescriptorHeapAllocator
	{
		ID3D12DescriptorHeap* Heap = nullptr;
		D3D12_DESCRIPTOR_HEAP_TYPE  HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		D3D12_CPU_DESCRIPTOR_HANDLE HeapStartCpu;
		D3D12_GPU_DESCRIPTOR_HANDLE HeapStartGpu;
		UINT                        HeapHandleIncrement;
		ImVector<int>               FreeIndices;

		void Create(ID3D12Device* device, ID3D12DescriptorHeap* heap)
		{
			IM_ASSERT(Heap == nullptr && FreeIndices.empty());
			Heap = heap;
			D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();
			HeapType = desc.Type;
			HeapStartCpu = Heap->GetCPUDescriptorHandleForHeapStart();
			//HeapStartGpu = Heap->GetGPUDescriptorHandleForHeapStart();
			HeapHandleIncrement = device->GetDescriptorHandleIncrementSize(HeapType);
			FreeIndices.reserve((int)desc.NumDescriptors);
			for (int n = desc.NumDescriptors; n > 0; n--)
				FreeIndices.push_back(n - 1);
		}
		void Destroy()
		{
			Heap = nullptr;
			FreeIndices.clear();
		}
		void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
		{
			IM_ASSERT(FreeIndices.Size > 0);
			int idx = FreeIndices.back();
			FreeIndices.pop_back();
			out_cpu_desc_handle->ptr = HeapStartCpu.ptr + (idx * HeapHandleIncrement);
			out_gpu_desc_handle->ptr = HeapStartGpu.ptr + (idx * HeapHandleIncrement);
		}
		void Free(D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle)
		{
			int cpu_idx = (int)((out_cpu_desc_handle.ptr - HeapStartCpu.ptr) / HeapHandleIncrement);
			int gpu_idx = (int)((out_gpu_desc_handle.ptr - HeapStartGpu.ptr) / HeapHandleIncrement);
			IM_ASSERT(cpu_idx == gpu_idx);
			FreeIndices.push_back(cpu_idx);
		}
	};

	// Data
	static FrameContext                 g_frameContext[APP_NUM_FRAMES_IN_FLIGHT] = {};
	static UINT                         g_frameIndex = -1;

	static ExampleDescriptorHeapAllocator g_pd3dSrvDescHeapAlloc;
	static ID3D12Fence* g_fence = nullptr;
	static HANDLE                       g_fenceEvent = nullptr;
	static UINT64                       g_fenceLastSignaledValue = 0;
	static bool                         g_SwapChainOccluded = false;
	static HANDLE                       g_hSwapChainWaitableObject = nullptr;
	//static ID3D12Resource* g_mainRenderTargetResource[APP_NUM_BACK_BUFFERS] = {};

	// Forward declarations of helper functions
	void WaitForLastSubmittedFrame(DX12& dx12);
	FrameContext* WaitForNextFrameResources(DX12& dx12);
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

namespace BoulderLeaf::Graphics::DX12
{
	ImguiExample::ImguiExample(std::shared_ptr<DX12> dx12) : AbstractExample(dx12),
		mSwapChainOccluded(false),
		showDemoWindow(false)
	{
		for (UINT i = 0; i < APP_NUM_FRAMES_IN_FLIGHT; i++)
		{
			//WHAT ARE THESE ALLOCATORS. WHy do I need more than 1? My Box example only has 1. 
			dx12->mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator));
		}

		auto result = dx12->mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence));
		assert(result == S_OK);

		g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		assert(g_fenceEvent != nullptr);
			

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		ImGui_ImplWin32_Init(dx12->mhMainWnd);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGui_ImplDX12_InitInfo info;
		info.Device = dx12->mDevice.Get();
		info.CommandQueue = dx12->mCommandQueue.Get();
		info.DSVFormat = sDepthStencilFormat;
		info.RTVFormat = sBackbufferFormat;
		info.SrvDescriptorHeap = dx12->mRtvHeap.Get();
		info.NumFramesInFlight = 1;
		info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
			{
				return g_pd3dSrvDescHeapAlloc.Alloc(out_cpu_desc_handle, out_gpu_desc_handle);
			};
		info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle)
			{
				g_pd3dSrvDescHeapAlloc.Free(cpu_desc_handle, gpu_desc_handle);
			};

		g_pd3dSrvDescHeapAlloc.Create(dx12->mDevice.Get(), dx12->mCbvHeap.Get());
		ImGui_ImplDX12_Init(&info);
	}

	ImguiExample::~ImguiExample()
	{

	}

	void ImguiExample::Update(const Metrics::blTime& gameTime)
	{

	}

	void ImguiExample::Draw()
	{
		DX12& dx12 = *m_dx12.get();

		// Handle window screen locked
		if (mSwapChainOccluded && dx12.mSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
		{
			return;
		}

		mSwapChainOccluded = false;

		// Start the Dear ImGui frame
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		// Rendering
		ImGui::Render();

		FrameContext* frameCtx = WaitForNextFrameResources(dx12);
		UINT backBufferIdx = g_frameIndex % APP_NUM_FRAMES_IN_FLIGHT;
		frameCtx->CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = dx12.mSwapChainBuffer[backBufferIdx].Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx12.mCommandList->Reset(frameCtx->CommandAllocator, nullptr);
		dx12.mCommandList->ResourceBarrier(1, &barrier);

		// Render Dear ImGui graphics
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_dx12->DepthStencilView();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		auto backBufferView = dx12.GetBackBufferView(backBufferIdx);
		dx12.mCommandList->ClearRenderTargetView(dx12.GetBackBufferView(backBufferIdx), clear_color_with_alpha, 0, nullptr);
		m_dx12->mCommandList->ClearDepthStencilView(m_dx12->DepthStencilView(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);

		dx12.mCommandList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
		
		//this function is asking for a pointer to an array. I should sore these as an array in DX12
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_dx12->mCbvHeap.Get() };
		dx12.mCommandList->SetDescriptorHeaps(1, descriptorHeaps);

		//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dx12.mCommandList.Get());
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx12.mCommandList->ResourceBarrier(1, &barrier);
		dx12.mCommandList->Close();

		ID3D12CommandList* cmdsLists[] = { dx12.mCommandList.Get() };
		m_dx12->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Present
		HRESULT hr = dx12.mSwapChain->Present(0, 0); // Present without vsync
		g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

		UINT64 fenceValue = g_fenceLastSignaledValue + 1;
		dx12.mCommandQueue->Signal(g_fence, fenceValue);
		g_fenceLastSignaledValue = fenceValue;
		frameCtx->FenceValue = fenceValue;
	}

	LRESULT WINAPI ImguiExample::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		DX12& dx12 = *m_dx12;

		switch (msg)
		{
		case WM_SIZE:
			if (dx12.mDevice != nullptr && wParam != SIZE_MINIMIZED)
			{
				WaitForLastSubmittedFrame(dx12);
				HRESULT result = dx12.mSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
				assert(SUCCEEDED(result) && "Failed to resize swapchain.");
			}
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}
		return ::DefWindowProcW(hWnd, msg, wParam, lParam);
	}
}

namespace
{

	void WaitForLastSubmittedFrame(DX12& dx12)
	{
		FrameContext* frameCtx = &g_frameContext[g_frameIndex % APP_NUM_FRAMES_IN_FLIGHT];

		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue == 0)
			return; // No fence was signaled

		frameCtx->FenceValue = 0;
		if (g_fence->GetCompletedValue() >= fenceValue)
			return;

		g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
		WaitForSingleObject(g_fenceEvent, INFINITE);
	}

	FrameContext* WaitForNextFrameResources(DX12& dx12)
	{
		UINT nextFrameIndex = g_frameIndex + 1;
		g_frameIndex = nextFrameIndex;

		HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, nullptr };
		DWORD numWaitableObjects = 1;

		FrameContext* frameCtx = &g_frameContext[nextFrameIndex % APP_NUM_FRAMES_IN_FLIGHT];
		UINT64 fenceValue = frameCtx->FenceValue;
		if (fenceValue != 0) // means no fence was signaled
		{
			frameCtx->FenceValue = 0;
			g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
			waitableObjects[1] = g_fenceEvent;
			numWaitableObjects = 2;
		}

		WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

		return frameCtx;
	}
}