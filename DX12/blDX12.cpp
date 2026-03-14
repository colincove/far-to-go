//BoulderLeaf
#include <blDX12.h>
#include <blSwapChain.h>
#include <blWindow.h>

//DirectX
#include "dxgi1_3.h"
#include <dxgidebug.h>
#include <DirectXColors.h>

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
			globalRenderDataRef.device,
			window,
			globalRenderDataRef.commandQueue,
			globalRenderDataRef.factory);

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

	void blDX12::StartFrameInternal()
	{
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have finished
		// execution on the GPU.
		mGlobalRenderDataPtr->commandListAllocator->Reset();

		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			GroupStartFrame(group);
		}

		for (int i = 0; i < C_ARRAY_COUNT(mRenderComponents); i++)
		{
			mRenderComponents[i]->StartFrame();
		}

		// Clear the back buffer and depth buffer.
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = mGlobalRenderDataPtr->depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = mGlobalRenderDataPtr->swapChain->CurrentBackBufferView();
	}

	void blDX12::EndFrameInternal()
	{
		std::vector<ID3D12CommandList*> cmdsLists;
		cmdsLists.reserve(blRenderGroups::GetCount());

		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			GroupEndFrame(group);	
			blRenderGroupData& groupData = mGlobalRenderDataPtr->renderGroupData[group];
			cmdsLists.push_back(groupData.commandList->GetCommandListPtr().Get());
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

	void blDX12::InitializeGroupInternal(const blRenderGroupId& group)
	{
		blRenderGroupData& dx12GroupData = mGlobalRenderDataPtr->renderGroupData[group];
		BoulderLeaf::Graphics::blRenderGroupData groupData = blRenderGroups::GetRenderGroupData(group);
		dx12GroupData.commandList = std::make_shared<blCommandList>(mGlobalRenderDataPtr->commandListAllocator);
		dx12GroupData.commandList->GetCommandListPtr()->SetName(groupData.NameWide.c_str());
		dx12GroupData.meshDataDeviceCache = std::make_shared<blDX12MeshDataDeviceCache>(
			mGlobalRenderDataPtr->device,
			dx12GroupData.commandList,
			mGlobalRenderDataPtr->meshStorageCache);

		ComPtr<ID3D12GraphicsCommandList> commandList = dx12GroupData.commandList->GetCommandListPtr();


		//When should I do this? it was not in the example
		//You cannot call Reset unless it is in a closed state
		DX12_API_CALL(commandList->Close());
		DX12_API_CALL(commandList->Reset(mGlobalRenderDataPtr->commandListAllocator->GetAllocatorPtr().Get(), nullptr));
	}

	void blDX12::InitializeBegin()
	{

	}

	void blDX12::InitializeFinish()
	{
		// Execute the initialization commands.
		//m_dx12->mCommandList->Close(); //I am assuming that I don't need to do this, beause I am doing it above. 
		std::vector<ID3D12CommandList*> cmdsLists;
		cmdsLists.reserve(blRenderGroups::GetCount());

		for (blRenderGroupId group : blRenderGroups::Iterator())
		{
			blRenderGroupData& groupData = mGlobalRenderDataPtr->renderGroupData[group];

			//again. Why do do Need to do this? I got an error.
			//Command lists must be closed before execution.
			groupData.commandList->GetCommandListPtr()->Close();
			cmdsLists.push_back(groupData.commandList->GetCommandListPtr().Get());
		}

		mGlobalRenderDataPtr->commandQueue->GetDX12CommandQueue()->ExecuteCommandLists(
			static_cast<UINT>(cmdsLists.size()),
			reinterpret_cast<ID3D12CommandList* const*>(cmdsLists.data()) // will this work? took it from LLM. 
		);

		// Wait until initialization is complete.
		FlushCommandQueue();
	}

	void blDX12::GroupStartFrame(blRenderGroupId group)
	{
		blRenderGroupData& groupData = mGlobalRenderDataPtr->renderGroupData[group];
		ComPtr<ID3D12GraphicsCommandList> commandList = groupData.commandList->GetCommandListPtr();

		// passing mCurrentPSO is garbage here. I am just trying to debug why the Input Assembler stage does 
		// not have the correct geometry in it. 
		commandList->Reset(mGlobalRenderDataPtr->commandListAllocator->GetAllocatorPtr().Get(), 
			mGlobalRenderDataPtr->mCurrentPSO ? mGlobalRenderDataPtr->mCurrentPSO->GetDX12PSO().Get() : nullptr);

		//TODO make sure we are setting up viewPort correctly. 
		commandList->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		commandList->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);

		// Indicate a state transition on the resource usage.
		ID3D12Resource* currentBackBuffer = mGlobalRenderDataPtr->swapChain->GetCurrentBackBuffer();
		CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer,
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &resourceBarrier);

		//intresting. if I have multiple command lists, do I need to clear the render target view for each one? That does not sound right. 
		//maybe you are only supposed to do this for your primary command list?
		commandList->ClearRenderTargetView(mGlobalRenderDataPtr->swapChain->CurrentBackBufferView(),
			Colors::LightSteelBlue, 0, nullptr);
		commandList->ClearDepthStencilView(mGlobalRenderDataPtr->depthBuffer->DepthStencilView(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);
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