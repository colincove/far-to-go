#include <blDX12Imgui.h>

#include <blRenderGroup.h>

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	using namespace BoulderLeaf::Graphics;
	using namespace BoulderLeaf::Graphics::DX12;

	bool showDemoWindow = true;

	//static blRenderGroupId ImguiRenderGroup = blRenderGroups::Default;

	// Config for example app
	static const int APP_NUM_BACK_BUFFERS = Constants::SwapChainBufferCount;
	static const int APP_SRV_HEAP_SIZE = Constants::SrvHeapSize;

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
			HeapStartGpu = Heap->GetGPUDescriptorHandleForHeapStart();
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

	static ExampleDescriptorHeapAllocator g_pd3dSrvDescHeapAlloc;
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12Imgui::blDX12Imgui(std::shared_ptr<blGlobalRenderData> globalRenderData,
		std::shared_ptr<Core::blWindow> window)
		: mGlobalRenderData(globalRenderData),
		mWindow(window)
	{

	}

	LRESULT blDX12Imgui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}

	void blDX12Imgui::Initialize()
	{
		//ImguiRenderGroup = BoulderLeaf::Graphics::blRenderGroups::RegisterExternal("Imgui");

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		ImGui_ImplWin32_Init(mWindow->GetWindowHandle());

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGui_ImplDX12_InitInfo info;
		info.Device = mGlobalRenderData->device->GetDX12Device().Get();
		info.CommandQueue = mGlobalRenderData->commandQueue->GetDX12CommandQueue().Get();
		info.DSVFormat = Constants::DepthStencilFormat;
		info.RTVFormat = Constants::BackbufferFormat;
		info.SrvDescriptorHeap = mGlobalRenderData->swapChain->GetRtvHeap().Get();
		info.NumFramesInFlight = 1;

		info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
			{
				return g_pd3dSrvDescHeapAlloc.Alloc(out_cpu_desc_handle, out_gpu_desc_handle);
			};

		info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_desc_handle)
			{
				g_pd3dSrvDescHeapAlloc.Free(cpu_desc_handle, gpu_desc_handle);
			};

		g_pd3dSrvDescHeapAlloc.Create(mGlobalRenderData->device->GetDX12Device().Get(), 
			mGlobalRenderData->constantBufferDescriptorHeap->GetDescriptorHeap().Get());

		ImGui_ImplDX12_Init(&info);
	}

	void blDX12Imgui::StartFrame()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (showDemoWindow)
		{
			ImGui::ShowDemoWindow(&showDemoWindow);
		}
	}

	void blDX12Imgui::EndFrame()
	{
		// Rendering
		ImGui::Render();

		blRenderGroupData& renderGroupData = mGlobalRenderData->renderGroupData[blRenderGroups::Default];
		//blRenderGroupData& renderGroupData = mGlobalRenderData->renderGroupData[ImguiRenderGroup];

		//this function is asking for a pointer to an array. I should sore these as an array in DX12
		ID3D12DescriptorHeap* descriptorHeaps[] = { mGlobalRenderData->constantBufferDescriptorHeap->GetDescriptorHeap().Get()};
		renderGroupData.commandList->GetCommandListPtr().Get()->SetDescriptorHeaps(1, descriptorHeaps);

		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderGroupData.commandList->GetCommandListPtr().Get());
	}
}