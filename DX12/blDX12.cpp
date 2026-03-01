//BoulderLeaf
#include <blDX12.h>
#include <blSwapChain.h>
#include <blWindow.h>

//DirectX
#include "dxgi1_3.h"
#include <dxgidebug.h>

//Standard
#include <atldef.h>
#include <assert.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12::blDX12(std::shared_ptr<Core::blWindow> window) :
		blGraphicsAPIImpl(window),
		mWindow(window),
		mGlobalRenderDataPtr(std::make_shared<blGlobalRenderData>()),
		mMeshRenderComponent(std::make_shared<blGlobalRenderData>()),
		mMeshInstancedRenderComponent(std::make_shared<blGlobalRenderData>()),
		mCurrentFence(0)
	{
#ifdef DEBUG
		ComPtr<ID3D12Debug> debugController;
		DX12_API_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (DX12_API_CALL_SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			//dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			DX12_API_CALL(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true));
			DX12_API_CALL(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true));
		}
#endif // DEBUG

		mMeshRenderComponent = blMeshRenderComponent(mGlobalRenderDataPtr);
		mMeshInstancedRenderComponent = blMeshInstancedRenderComponent(mGlobalRenderDataPtr);

		blGlobalRenderData& globalRenderDataRef = *mGlobalRenderDataPtr.get();

		globalRenderDataRef.viewPort.TopLeftX = 0;
		globalRenderDataRef.viewPort.TopLeftY = 0;
		globalRenderDataRef.viewPort.Width = static_cast<float>(window->GetWidth());
		globalRenderDataRef.viewPort.Height = static_cast<float>(window->GetHeight());
		globalRenderDataRef.viewPort.MinDepth = 0;
		globalRenderDataRef.viewPort.MaxDepth = 1;

		globalRenderDataRef.scissorRect = { 0, 0, static_cast<long>(window->GetWidth()), static_cast<long>(window->GetHeight()) };

		globalRenderDataRef.device = std::make_shared<blDevice>();
		globalRenderDataRef.commandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataRef.device);
		globalRenderDataRef.commandQueue = std::make_shared<blCommandQueue>(globalRenderDataRef.device);
		globalRenderDataRef.factory = std::make_shared<blFactory>();

		globalRenderDataRef.swapChain = std::make_shared<blSwapChain>(
			2,
			globalRenderDataRef.device);

		globalRenderDataRef.shaderCache = std::make_shared<blShaderCache>(globalRenderDataRef.device);
		globalRenderDataRef.meshStorageCache = std::make_shared<blDX12MeshStorageCache>(globalRenderDataRef.device);
		globalRenderDataRef.mPSOCache = std::make_shared<blPSOCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.shaderCache);
		globalRenderDataRef.bufferCache = std::make_shared<blDX12BufferDataCache>(globalRenderDataRef.device);
		
		globalRenderDataRef.depthBuffer = std::make_shared<blDepthBuffer>(globalRenderDataRef.device, window);
		globalRenderDataRef.fence = std::make_shared<blFence>(globalRenderDataRef.device);
		globalRenderDataRef.constantBufferDescriptorHeap = std::make_shared<blConstantBufferDescriptorHeap>(globalRenderDataRef.device);

		globalRenderDataRef.constantBufferCache = std::make_shared<blDX12ConstantBufferCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.constantBufferDescriptorHeap,
			globalRenderDataRef.bufferCache);

		// Wait until initialization is complete.
		FlushCommandQueue();
	}

	blDX12::~blDX12()
	{
		FlushCommandQueue();
	}

	void blDX12::OnWindowMessage(MSG msg)
	{

	}

	void blDX12::OnResize()
	{

	}

	void blDX12::StartFrame()
	{
		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			if (IsGroupInitialized(group))
			{
				GroupStartFrame(group);
			}
		}

		for (int i = 0; i < C_ARRAY_COUNT(mRenderComponents); i++)
		{
			mRenderComponents[i]->StartFrame();
		}
	}

	void blDX12::EndFrame()
	{
		std::vector<ID3D12CommandList*> cmdsLists;
		cmdsLists.reserve(blRenderGroups::GetCount());

		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			if (IsGroupInitialized(group))
			{
				GroupEndFrame(group);		
				blRenderGroupData& groupData = mGlobalRenderDataPtr->renderGroupData[group];
				cmdsLists.push_back(groupData.commandList->GetCommandListPtr().Get());
			}
		}

		mGlobalRenderDataPtr->commandQueue->GetDX12CommandQueue()->ExecuteCommandLists(
			static_cast<UINT>(cmdsLists.size()), 
			reinterpret_cast<ID3D12CommandList* const*>(cmdsLists.data()) // will this work? took it from LLM. 
		);

		mGlobalRenderDataPtr->swapChain->Present();
		FlushCommandQueue();
	}

	void blDX12::DrawMesh(const RenderMeshData& renderItem, const blSceneResourcePtr scene)
	{
		mMeshRenderComponent.Render(renderItem, scene);
	}

	void blDX12::DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene)
	{
		mMeshInstancedRenderComponent.Render(renderData, scene);
	}

	void blDX12::InitializeGroup(const blRenderGroupId& group)
	{
		blRenderGroupData& dx12GroupData = mGlobalRenderDataPtr->renderGroupData[group];
		BoulderLeaf::Graphics::blRenderGroupData groupData = blRenderGroups::GetRenderGroupData(group);
		dx12GroupData.commandList = std::make_shared<blCommandList>(mGlobalRenderDataPtr->commandListAllocator);
		dx12GroupData.commandList->GetCommandListPtr()->SetName(groupData.NameWide.c_str());
		dx12GroupData.meshDataDeviceCache = std::make_shared<blDX12MeshDataDeviceCache>(
			mGlobalRenderDataPtr->device,
			dx12GroupData.commandList,
			mGlobalRenderDataPtr->meshStorageCache);

		GroupStartFrame(group);
	}

	void blDX12::GroupStartFrame(blRenderGroupId group)
	{
		blRenderGroupData& groupData = mGlobalRenderDataPtr->renderGroupData[group];
		ComPtr<ID3D12GraphicsCommandList> commandList = groupData.commandList->GetCommandListPtr();
		commandList->Reset(mGlobalRenderDataPtr->commandListAllocator->GetAllocatorPtr().Get(), nullptr);

		//TODO make sure we are setting up viewPort correctly. 
		commandList->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		commandList->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);
	}

	void blDX12::GroupEndFrame(blRenderGroupId group)
	{
		blRenderGroupData& groupData = mGlobalRenderDataPtr->renderGroupData[group];
		ID3D12Resource* currentBackBuffer = mGlobalRenderDataPtr->swapChain->GetCurrentBackBuffer();
		ComPtr<ID3D12GraphicsCommandList> commandList = groupData.commandList->GetCommandListPtr();
		// TODO present and wait for GPU
		CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer,
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1,
			&resourceBarrier);
		// Done recording commands.
		commandList->Close();
	}

	// Wait until frame commands are complete. This waiting is
	// inefficient and is done for simplicity. Later we will show how to
	// organize our rendering code so we do not have to wait per frame.
	void blDX12::FlushCommandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		mCurrentFence++;
		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to
		// this Signal().
		mGlobalRenderDataPtr->commandQueue->GetDX12CommandQueue()->Signal(
			mGlobalRenderDataPtr->fence->GetFence().Get(), mCurrentFence);
		// Wait until the GPU has completed commands up to this fence point.
		if (mGlobalRenderDataPtr->fence->GetFence()->GetCompletedValue() < mCurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			// Fire event when GPU hits current fence.
			mGlobalRenderDataPtr->fence->GetFence()->SetEventOnCompletion(mCurrentFence, eventHandle);
			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}
}