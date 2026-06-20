//BoulderLeaf
#include <blDX12.h>
#include <blSwapChain.h>
#include <blWindow.h>

//DirectX
#include "dxgi1_3.h"
#include <dxgidebug.h>
#include <DirectXColors.h>

//Standard
#include <assert.h>

namespace
{
	bool imguiEnabled = true;
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12::blDX12(std::shared_ptr<Core::blWindow> window) :
		blGraphicsAPIImpl(window),
		mWindow(window),
		mGlobalRenderDataPtr(std::make_shared<blGlobalRenderData>()),
		mMeshRenderComponent(),
		mMeshInstancedRenderComponent(),
		mCompositeMeshRenderComponent(),
		mCurrentFence(0),
		mDX12Imgui(std::make_shared<blDX12Imgui>(mGlobalRenderDataPtr, mWindow))
	{
		//this is not working for me right now. not sure why. some build issues. 
//#ifdef DEBUG
#if 1
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

		blGlobalRenderData& globalRenderDataRef = *mGlobalRenderDataPtr.get();

		globalRenderDataRef.viewPort.TopLeftX = 0;
		globalRenderDataRef.viewPort.TopLeftY = 0;
		globalRenderDataRef.viewPort.Width = static_cast<float>(window->GetWidth());
		globalRenderDataRef.viewPort.Height = static_cast<float>(window->GetHeight());
		globalRenderDataRef.viewPort.MinDepth = 0;
		globalRenderDataRef.viewPort.MaxDepth = 1;

		globalRenderDataRef.scissorRect = { 0, 0, static_cast<long>(window->GetWidth()), static_cast<long>(window->GetHeight()) };

		globalRenderDataRef.device = std::make_shared<blDevice>();
		globalRenderDataRef.commandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataRef.device, L"Global");
		globalRenderDataRef.commandList = std::make_shared<blCommandList>(mGlobalRenderDataPtr->commandListAllocator, L"Default");
		globalRenderDataRef.commandListAllocator2 = std::make_shared<blCommandListAllocator>(globalRenderDataRef.device, L"Global");
		globalRenderDataRef.imguiCommandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataRef.device, L"Imgui");
		globalRenderDataRef.commandList2 = std::make_shared<blCommandList>(mGlobalRenderDataPtr->commandListAllocator2, L"Default");
		globalRenderDataRef.imgguiCommandList = std::make_shared<blCommandList>(mGlobalRenderDataPtr->imguiCommandListAllocator, L"Imgui");
		globalRenderDataRef.commandQueue = std::make_shared<blCommandQueue>(globalRenderDataRef.device, L"Global");
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
		globalRenderDataRef.fence = std::make_shared<blFence>(globalRenderDataRef.device, L"Global");
		globalRenderDataRef.constantBufferDescriptorHeap = std::make_shared<blConstantBufferDescriptorHeap>(
			globalRenderDataRef.device,
			L"Global");

		globalRenderDataRef.constantBufferCache = std::make_shared<blDX12ConstantBufferCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.constantBufferDescriptorHeap,
			globalRenderDataRef.bufferCache);

		mResourceCaches = {
			globalRenderDataRef.shaderCache,
			globalRenderDataRef.meshStorageCache,
			globalRenderDataRef.constantBufferCache,
			globalRenderDataRef.bufferCache,
			globalRenderDataRef.mPSOCache
		};

		mMeshRenderComponent = std::make_unique <blMeshRenderComponent>(mGlobalRenderDataPtr);
		mMeshInstancedRenderComponent = std::make_unique<blMeshInstancedRenderComponent>(mGlobalRenderDataPtr);
		mCompositeMeshRenderComponent = std::make_unique<blCompositeMeshRenderComponent>(mGlobalRenderDataPtr);

		//this vector is kind of dumb. Does not scale. And holds raw pointers to components that are owned by unique_ptrs. But it is what it is for now. We can optimize this later if we need to.
		mRenderComponents = std::vector<blRenderComponentBase*>
		{
			mMeshRenderComponent.get(),
			mMeshInstancedRenderComponent.get(),
			mMeshInstancedRenderComponent.get()
		};

		// Wait until initialization is complete.
		FlushCommandQueue();
	}

	blDX12::~blDX12()
	{
		FlushCommandQueue();
	}

	LRESULT blDX12::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return mDX12Imgui->WndProc(hWnd, msg, wParam, lParam);
	}

	void blDX12::OnWindowMessage(MSG msg)
	{

	}

	void blDX12::OnResize()
	{

	}

	void blDX12::InitializeInternal()
	{
		if (imguiEnabled)
		{
			mDX12Imgui->Initialize();
		}

		// Execute the initialization commands.
		//m_dx12->mCommandList->Close(); //I am assuming that I don't need to do this, beause I am doing it above. 
		std::vector<ID3D12CommandList*> cmdsLists;

		mGlobalRenderDataPtr->commandList->Close();
		mGlobalRenderDataPtr->commandList2->Close();
		mGlobalRenderDataPtr->imgguiCommandList->Close();
		mMeshInstancedRenderComponent->GetCommandList()->Close();

		// Wait until initialization is complete.
		FlushCommandQueue();

		mGlobalRenderDataPtr->meshDataDeviceCache = std::make_shared<blDX12MeshDataDeviceCache>(
			mGlobalRenderDataPtr->device,
			mGlobalRenderDataPtr->commandList,
			mGlobalRenderDataPtr->meshStorageCache);

		mGlobalRenderDataPtr->compositeMeshStorageCache = std::make_shared<blCompositeMeshDataCache>(
			mGlobalRenderDataPtr->device,
			mGlobalRenderDataPtr->commandList,
			mGlobalRenderDataPtr->meshStorageCache);
	}

	void blDX12::StartFrameInternal()
	{
		// TODO: We can optimize this by only updating the caches that are associated with the dirty resources.
		for (std::shared_ptr<blDX12ResourceCacheBase> cache : mResourceCaches)
		{
			for (blResourceId resourceId : mDirtyResources)
			{
				if (cache->IsCacheInitialized(resourceId))
				{
					cache->UpdateCache(resourceId);
				}
			}
		}

		mDirtyResources.clear();

		for (blRenderComponentBase* renderComponent : mRenderComponents)
		{
			//renderComponent->StartFrame();
		}
		mGlobalRenderDataPtr->imguiCommandListAllocator->Reset();
		mGlobalRenderDataPtr->imgguiCommandList->Reset();

		mMeshInstancedRenderComponent->GetCommandListAllocator()->Reset();
		mMeshInstancedRenderComponent->GetCommandList()->Reset();

		mGlobalRenderDataPtr->commandListAllocator->Reset();
		mGlobalRenderDataPtr->commandList->Reset();

		mGlobalRenderDataPtr->commandListAllocator2->Reset();
		mGlobalRenderDataPtr->commandList2->Reset();

		if (imguiEnabled)
		{
			mDX12Imgui->StartFrame();
		}

		//TODO make sure we are setting up viewPort correctly. 
		mGlobalRenderDataPtr->commandList->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		mGlobalRenderDataPtr->commandList->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);

		mGlobalRenderDataPtr->commandList2->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		mGlobalRenderDataPtr->commandList2->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);
		mMeshInstancedRenderComponent->GetCommandList()->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		mMeshInstancedRenderComponent->GetCommandList()->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);
		mGlobalRenderDataPtr->imgguiCommandList->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		mGlobalRenderDataPtr->imgguiCommandList->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);

		// Indicate a state transition on the resource usage.
		ID3D12Resource* currentBackBuffer = mGlobalRenderDataPtr->swapChain->GetCurrentBackBuffer();

		D3D12_RESOURCE_BARRIER resourceBarrier = {};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = currentBackBuffer;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		mGlobalRenderDataPtr->commandList->ResourceBarrier(1, &resourceBarrier);
		mGlobalRenderDataPtr->commandList->ClearRenderTargetView(mGlobalRenderDataPtr->swapChain->CurrentBackBufferView(),
			Colors::LightSteelBlue, 0, nullptr);
		mGlobalRenderDataPtr->commandList->ClearDepthStencilView(mGlobalRenderDataPtr->depthBuffer->DepthStencilView(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);
	}

	void blDX12::EndFrameInternal()
	{
		if (imguiEnabled)
		{
			mDX12Imgui->EndFrame();
		}

		mMeshInstancedRenderComponent->GetCommandList()->Close();
		// Done recording commands.
		

		ID3D12Resource* currentBackBuffer = mGlobalRenderDataPtr->swapChain->GetCurrentBackBuffer();
		std::shared_ptr<blCommandList> commandList = mGlobalRenderDataPtr->commandList2;
		// TODO present and wait for GPU
		CD3DX12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer,
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1,
			&resourceBarrier);

		// Done recording commands.
		mGlobalRenderDataPtr->commandList->Close();
		mGlobalRenderDataPtr->commandList2->Close();
		mGlobalRenderDataPtr->imgguiCommandList->Close();

		mGlobalRenderDataPtr->commandQueue->ExecuteCommandLists(
			std::vector<std::shared_ptr<blCommandList>> {mGlobalRenderDataPtr->commandList}
		);

		mGlobalRenderDataPtr->commandQueue->ExecuteCommandLists(
			std::vector<std::shared_ptr<blCommandList>> {
				mMeshInstancedRenderComponent->GetCommandList(),
				mGlobalRenderDataPtr->imgguiCommandList}
		);

		mGlobalRenderDataPtr->commandQueue->ExecuteCommandLists(
			std::vector<std::shared_ptr<blCommandList>> {mGlobalRenderDataPtr->commandList2}
		);

		mGlobalRenderDataPtr->swapChain->Present();
		FlushCommandQueue();
	}

	void blDX12::DrawMesh(const RenderMeshData& renderItem, const blSceneResourcePtr scene)
	{
		mMeshRenderComponent->Render(renderItem, scene);
	}

	void blDX12::DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene)
	{
		mMeshInstancedRenderComponent->Render(renderData, scene);
	}

	void blDX12::DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData, const blSceneResourcePtr scene)
	{
		mCompositeMeshRenderComponent->Render(renderData, scene);
	}

	void blDX12::MarkResourceDirty(const blResourceId resourceId)
	{
		mDirtyResources.insert(resourceId);
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