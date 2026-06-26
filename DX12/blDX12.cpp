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

#include <blDX12ConstantBufferWithPassCache.h>
#include <blDX12ElementUploadBufferCache.h>

namespace
{
	#define RENDER_COMPONENT_FUNCTION_CALL(functionName) \
	for (blRenderComponentBase* renderComponent : mRenderComponents) \
	{ \
		renderComponent->functionName(); \
	}
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
		mCompositeMeshRenderWithPassConstantsRenderComponent(),
		mCurrentFence(0)
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

		globalRenderDataRef.globalRenderFrameContext = std::make_shared<blGlobalRenderFrameContext>();

		globalRenderDataRef.viewPort.TopLeftX = 0;
		globalRenderDataRef.viewPort.TopLeftY = 0;
		globalRenderDataRef.viewPort.Width = static_cast<float>(window->GetWidth());
		globalRenderDataRef.viewPort.Height = static_cast<float>(window->GetHeight());
		globalRenderDataRef.viewPort.MinDepth = 0;
		globalRenderDataRef.viewPort.MaxDepth = 1;

		globalRenderDataRef.scissorRect = { 0, 0, static_cast<long>(window->GetWidth()), static_cast<long>(window->GetHeight()) };

		globalRenderDataRef.device = std::make_shared<blDevice>();

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
		globalRenderDataRef.bufferCache = std::make_shared<blDX12BufferDataCache>(
			globalRenderDataRef.device, 
			globalRenderDataRef.globalRenderFrameContext);
		globalRenderDataRef.vertexAndPassUploadBufferCache = std::make_shared<blDX12VertexAndPassUploadBufferCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.globalRenderFrameContext);
		globalRenderDataRef.bufferElementCache = std::make_shared<blDX12BufferElementCache>();
		
		globalRenderDataRef.depthBuffer = std::make_shared<blDepthBuffer>(globalRenderDataRef.device, window);

		globalRenderDataRef.constantBufferCache = std::make_shared<blDX12ConstantBufferCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.bufferCache,
			globalRenderDataRef.globalRenderFrameContext);

		globalRenderDataRef.elementUploadBufferCache = std::make_shared<blDX12ElementUploadBufferCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.globalRenderFrameContext,
			globalRenderDataRef.bufferElementCache
		);

		globalRenderDataRef.constantBufferWithPassCache = std::make_shared<blDX12ConstantBufferWithPassCache>(
			globalRenderDataRef.device,
			globalRenderDataRef.bufferCache,
			globalRenderDataRef.bufferElementCache,
			globalRenderDataRef.globalRenderFrameContext,
			globalRenderDataRef.elementUploadBufferCache);

		mResourceCaches = {
			globalRenderDataRef.shaderCache,
			globalRenderDataRef.meshStorageCache,
			globalRenderDataRef.bufferElementCache,
			globalRenderDataRef.constantBufferCache,
			globalRenderDataRef.bufferCache,
			globalRenderDataRef.mPSOCache
		};

		for (FrameData& frameData : mFrameData)
		{
			frameData.mCommandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataRef.device, L"Global");
		}

		mFence = std::make_shared<blFence>(globalRenderDataRef.device, L"Global");
		mCommandList = std::make_shared<blCommandList>(
			mFrameData[globalRenderDataRef.globalRenderFrameContext->currentFrameResource].mCommandListAllocator, L"Default");

		mMeshRenderComponent = std::make_unique <blMeshRenderComponent>(mGlobalRenderDataPtr);
		mMeshInstancedRenderComponent = std::make_unique<blMeshInstancedRenderComponent>(mGlobalRenderDataPtr);
		mCompositeMeshRenderComponent = std::make_unique<blCompositeMeshRenderComponent>(mGlobalRenderDataPtr);
		mDX12ImguiRenderComponent = std::make_shared<blDX12Imgui>(mGlobalRenderDataPtr, mWindow);
		mCompositeMeshRenderWithPassConstantsRenderComponent = std::make_unique<blCompositeMeshRenderWithPassConstantsRenderComponent>(mGlobalRenderDataPtr);

		//this vector is kind of dumb. Does not scale. And holds raw pointers to components that are owned by unique_ptrs. But it is what it is for now. We can optimize this later if we need to.
		mRenderComponents = std::vector<blRenderComponentBase*>
		{
			//mMeshRenderComponent.get(), //not functioning right now. 
			mMeshInstancedRenderComponent.get(),
			mDX12ImguiRenderComponent.get(),
			mCompositeMeshRenderComponent.get(),
			mCompositeMeshRenderWithPassConstantsRenderComponent.get()
		};
	}

	blDX12::~blDX12()
	{
		FlushCommandQueue();
	}

	LRESULT blDX12::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return mDX12ImguiRenderComponent->WndProc(hWnd, msg, wParam, lParam);
	}

	void blDX12::OnWindowMessage(MSG msg)
	{

	}

	void blDX12::OnResize()
	{

	}

	void blDX12::InitializeInternal()
	{
		RENDER_COMPONENT_FUNCTION_CALL(Initialize)

		mCommandList->Close();

		mGlobalRenderDataPtr->meshDataDeviceCache = std::make_shared<blDX12MeshDataDeviceCache>(
			mGlobalRenderDataPtr->device,
			mCommandList, //this should work. Buuuuuut. I need to figure out more about how the command list is related to this cache. I might be missing something. 
			mGlobalRenderDataPtr->meshStorageCache);

		mGlobalRenderDataPtr->compositeMeshStorageCache = std::make_shared<blCompositeMeshDataCache>(
			mGlobalRenderDataPtr->device,
			mCommandList, //this should work. Buuuuuut. I need to figure out more about how the command list is related to this cache. I might be missing something. 
			mGlobalRenderDataPtr->meshStorageCache);
	}

	void blDX12::UpdateInternal(const Metrics::blTime& time)
	{
		mGlobalRenderDataPtr->globalRenderFrameContext->currentGameTime = time;
		mGlobalRenderDataPtr->globalRenderFrameContext->currentFrameResource = (Constants::FrameResourceCount + time.Tick()) % Constants::FrameResourceCount;

		FrameData& frameData = mFrameData[mGlobalRenderDataPtr->globalRenderFrameContext->currentFrameResource];

		// Has the GPU finished processing the commands of the current frame
		// resource. If not, wait until the GPU has completed commands up to
		// this fence point.
		if (frameData.mFence != 0 &&
			mFence->GetCompletedValue() < frameData.mFence)
			//mCommandQueue->GetLastCompletedFence() < mCurrFrameResource->Fence)
			//book had this. but the function does not exist. I did find it documented...somewhere on the internet. 
			//https://learn.microsoft.com/en-us/previous-versions/dn788633(v=vs.85)
			//but it appears to be old. 
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			mFence->SetEventOnCompletion(frameData.mFence, eventHandle);
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
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

		RENDER_COMPONENT_FUNCTION_CALL(StartFrame)

		FrameData& frameData = mFrameData[mGlobalRenderDataPtr->globalRenderFrameContext->currentFrameResource];
		frameData.mCommandListAllocator->Reset();
		mCommandList->Reset(frameData.mCommandListAllocator);

		//TODO make sure we are setting up viewPort correctly. 
		mCommandList->RSSetViewports(1, &mGlobalRenderDataPtr->viewPort);
		mCommandList->RSSetScissorRects(1, &mGlobalRenderDataPtr->scissorRect);

		// Indicate a state transition on the resource usage.
		ID3D12Resource* currentBackBuffer = mGlobalRenderDataPtr->swapChain->GetCurrentBackBuffer();

		D3D12_RESOURCE_BARRIER resourceBarrier = {};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = currentBackBuffer;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		mCommandList->ResourceBarrier(1, &resourceBarrier);
		mCommandList->ClearRenderTargetView(mGlobalRenderDataPtr->swapChain->CurrentBackBufferView(),
			Colors::LightSteelBlue, 0, nullptr);
		mCommandList->ClearDepthStencilView(mGlobalRenderDataPtr->depthBuffer->DepthStencilView(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);
	}

	void blDX12::EndFrameInternal()
	{
		RENDER_COMPONENT_FUNCTION_CALL(EndFrame)

		// Done recording commands.
		mCommandList->Close();

		mGlobalRenderDataPtr->commandQueue->ExecuteCommandLists(
			std::vector<std::shared_ptr<blCommandList>> {mCommandList}
		);

		mGlobalRenderDataPtr->commandQueue->ExecuteCommandLists(
			std::vector<std::shared_ptr<blCommandList>> {
				mMeshInstancedRenderComponent->GetCommandList(),
				mDX12ImguiRenderComponent->GetCommandList(),
				mCompositeMeshRenderComponent->GetCommandList(),
				mCompositeMeshRenderWithPassConstantsRenderComponent->GetCommandList()
			}
		);

		FrameData& frameData = mFrameData[mGlobalRenderDataPtr->globalRenderFrameContext->currentFrameResource];
		mCommandList->Reset(frameData.mCommandListAllocator);

		D3D12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			mGlobalRenderDataPtr->swapChain->GetCurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		mCommandList->ResourceBarrier(1,
			&resourceBarrier);

		mCommandList->Close();

		mGlobalRenderDataPtr->commandQueue->ExecuteCommandLists(
			std::vector<std::shared_ptr<blCommandList>> {mCommandList}
		);

		mGlobalRenderDataPtr->swapChain->Present();

		// Advance the fence value to mark commands up to this fence point.
		frameData.mFence = ++mCurrentFence;
		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to
		// this Signal().
		mGlobalRenderDataPtr->commandQueue->GetDX12CommandQueue()->Signal(
			mFence->Get(), mCurrentFence);
		//FlushCommandQueue();
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

	void blDX12::DrawCompositeMeshWithPass(const RenderCompositeMeshDataWithPassConstants& renderData, const blSceneResourcePtr scene)
	{
		mCompositeMeshRenderWithPassConstantsRenderComponent->Render(renderData, scene);
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
		mGlobalRenderDataPtr->commandQueue->Signal(
			mFence->Get(), mCurrentFence);

		// Wait until the GPU has completed commands up to this fence point.
		if (mFence->GetCompletedValue() < mCurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			// Fire event when GPU hits current fence.
			mFence->SetEventOnCompletion(mCurrentFence, eventHandle);
			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}
}