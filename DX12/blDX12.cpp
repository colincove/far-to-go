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
	blDX12::blDX12(Core::blWindow* window) :
		blGraphicsAPIImpl(window),
		mWindow(window),
		mGlobalRenderData(),
		mMeshRenderComponent(),
		mMeshInstancedRenderComponent(),
		mCompositeMeshRenderComponent(),
		mCompositeMeshRenderWithPassConstantsRenderComponent(),
		mCurrentFence(0),
		mResourceCacheGlobalInterface()
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

		mGlobalRenderData.globalRenderFrameContext = std::make_unique<blGlobalRenderFrameContext>();

		mGlobalRenderData.viewPort.TopLeftX = 0;
		mGlobalRenderData.viewPort.TopLeftY = 0;
		mGlobalRenderData.viewPort.Width = static_cast<float>(window->GetWidth());
		mGlobalRenderData.viewPort.Height = static_cast<float>(window->GetHeight());
		mGlobalRenderData.viewPort.MinDepth = 0;
		mGlobalRenderData.viewPort.MaxDepth = 1;

		mGlobalRenderData.scissorRect = { 0, 0, static_cast<long>(window->GetWidth()), static_cast<long>(window->GetHeight()) };

		mGlobalRenderData.device = std::make_unique<blDevice>();	

		mGlobalRenderData.commandQueue = std::make_unique<blCommandQueue>(mGlobalRenderData.device.get(), L"Global");
		mGlobalRenderData.factory = std::make_unique<blFactory>();

		mGlobalRenderData.swapChain = std::make_unique<blSwapChain>(
			2,
			mGlobalRenderData.device.get(),
			window,
			mGlobalRenderData.commandQueue.get(),
			mGlobalRenderData.factory.get());

		mGlobalRenderData.shaderCache = std::make_unique<blShaderCache>(mGlobalRenderData.device.get());
		mGlobalRenderData.meshStorageCache = std::make_unique<blDX12MeshStorageCache>(mGlobalRenderData.device.get());
		mGlobalRenderData.mPSOCache = std::make_unique<blPSOCache>(
			mGlobalRenderData.device.get(),
			mGlobalRenderData.shaderCache.get());
		mGlobalRenderData.bufferCache = std::make_unique<blDX12BufferDataCache>(
			mGlobalRenderData.device.get(),
			mGlobalRenderData.globalRenderFrameContext.get());
		mGlobalRenderData.vertexAndPassUploadBufferCache = std::make_unique<blDX12VertexAndPassUploadBufferCache>(
			mGlobalRenderData.device.get(),
			mGlobalRenderData.globalRenderFrameContext.get());
		mGlobalRenderData.bufferElementCache = std::make_unique<blDX12BufferElementCache>();
		
		mGlobalRenderData.depthBuffer = std::make_unique<blDepthBuffer>(mGlobalRenderData.device.get(), window);

		mGlobalRenderData.constantBufferCache = std::make_unique<blDX12ConstantBufferCache>(
			mGlobalRenderData.device.get(),
			mGlobalRenderData.bufferCache.get(),
			mGlobalRenderData.globalRenderFrameContext.get());

		mGlobalRenderData.elementUploadBufferCache = std::make_unique<blDX12ElementUploadBufferCache>(
			mGlobalRenderData.device.get(),
			mGlobalRenderData.globalRenderFrameContext.get(),
			mGlobalRenderData.bufferElementCache.get()
		);

		mGlobalRenderData.constantBufferWithPassCache = std::make_unique<blDX12ConstantBufferWithPassCache>(
			mGlobalRenderData.device.get(),
			mGlobalRenderData.bufferCache.get(),
			mGlobalRenderData.bufferElementCache.get(),
			mGlobalRenderData.globalRenderFrameContext.get(),
			mGlobalRenderData.elementUploadBufferCache.get());

		mResourceCaches = {
			mGlobalRenderData.shaderCache.get(),
			mGlobalRenderData.meshStorageCache.get(),
			mGlobalRenderData.bufferElementCache.get(),
			mGlobalRenderData.constantBufferCache.get(),
			mGlobalRenderData.bufferCache.get(),
			mGlobalRenderData.mPSOCache.get()
		};

		mResourceCacheGlobalInterface = std::make_unique<blDX12ResourceCacheGlobalInterface>(&mGlobalRenderData);
		mGlobalRenderData.resourceCacheGlobalInterface = mResourceCacheGlobalInterface.get();

		for (FrameData& frameData : mFrameData)
		{
			frameData.mCommandListAllocator = std::make_unique<blCommandListAllocator>(mGlobalRenderData.device.get(), L"Global");
		}

		mFence = std::make_unique<blFence>(mGlobalRenderData.device.get(), L"Global");
		mCommandList = std::make_unique<blCommandList>(
			mFrameData[mGlobalRenderData.globalRenderFrameContext->currentFrameResource].mCommandListAllocator.get(), L"Default");

		mMeshRenderComponent = std::make_unique <blMeshRenderComponent>(&mGlobalRenderData);
		mMeshInstancedRenderComponent = std::make_unique<blMeshInstancedRenderComponent>(&mGlobalRenderData);
		mCompositeMeshRenderComponent = std::make_unique<blCompositeMeshRenderComponent>(&mGlobalRenderData);
		mDX12ImguiRenderComponent = std::make_shared<blDX12Imgui>(&mGlobalRenderData, mWindow);
		mCompositeMeshRenderWithPassConstantsRenderComponent = std::make_unique<blCompositeMeshRenderWithPassConstantsRenderComponent>(&mGlobalRenderData);

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
	}

	void blDX12::UpdateInternal(const Metrics::blTime& time)
	{
		mGlobalRenderData.globalRenderFrameContext->currentGameTime = time;
		mGlobalRenderData.globalRenderFrameContext->currentFrameResource = (Constants::FrameResourceCount + time.Tick()) % Constants::FrameResourceCount;

		FrameData& frameData = mFrameData[mGlobalRenderData.globalRenderFrameContext->currentFrameResource];

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
		for (blDX12ResourceCacheBase* cache : mResourceCaches)
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

		FrameData& frameData = mFrameData[mGlobalRenderData.globalRenderFrameContext->currentFrameResource];
		frameData.mCommandListAllocator->Reset();
		mCommandList->Reset(frameData.mCommandListAllocator.get());

		//TODO make sure we are setting up viewPort correctly. 
		mCommandList->RSSetViewports(1, &mGlobalRenderData.viewPort);
		mCommandList->RSSetScissorRects(1, &mGlobalRenderData.scissorRect);

		// Indicate a state transition on the resource usage.
		ID3D12Resource* currentBackBuffer = mGlobalRenderData.swapChain->GetCurrentBackBuffer();

		D3D12_RESOURCE_BARRIER resourceBarrier = {};
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = currentBackBuffer;
		resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		mCommandList->ResourceBarrier(1, &resourceBarrier);
		mCommandList->ClearRenderTargetView(mGlobalRenderData.swapChain->CurrentBackBufferView(),
			Colors::LightSteelBlue, 0, nullptr);
		mCommandList->ClearDepthStencilView(mGlobalRenderData.depthBuffer->DepthStencilView(),
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f, 0, 0, nullptr);
	}

	void blDX12::EndFrameInternal()
	{
		RENDER_COMPONENT_FUNCTION_CALL(EndFrame)

		// Done recording commands.
		mCommandList->Close();

		mGlobalRenderData.commandQueue->ExecuteCommandLists(
			std::vector<blCommandList*> {mCommandList.get()}
		);

		mGlobalRenderData.commandQueue->ExecuteCommandLists(
			std::vector<blCommandList*> {
				mMeshInstancedRenderComponent->GetCommandList(),
				mDX12ImguiRenderComponent->GetCommandList(),
				mCompositeMeshRenderComponent->GetCommandList(),
				mCompositeMeshRenderWithPassConstantsRenderComponent->GetCommandList()
			}
		);

		FrameData& frameData = mFrameData[mGlobalRenderData.globalRenderFrameContext->currentFrameResource];
		mCommandList->Reset(frameData.mCommandListAllocator.get());

		D3D12_RESOURCE_BARRIER resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
			mGlobalRenderData.swapChain->GetCurrentBackBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		mCommandList->ResourceBarrier(1,
			&resourceBarrier);

		mCommandList->Close();

		mGlobalRenderData.commandQueue->ExecuteCommandLists(
			std::vector<blCommandList*> {mCommandList.get()}
		);

		mGlobalRenderData.swapChain->Present();

		// Advance the fence value to mark commands up to this fence point.
		frameData.mFence = ++mCurrentFence;
		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to
		// this Signal().
		mGlobalRenderData.commandQueue->GetDX12CommandQueue()->Signal(
			mFence->Get(), mCurrentFence);
	}

	void blDX12::DrawMesh(const RenderMeshData& renderItem)
	{
		mMeshRenderComponent->Render(renderItem);
	}

	void blDX12::DrawMeshInstanced(const RenderMeshDataInstanced& renderData)
	{
		mMeshInstancedRenderComponent->Render(renderData);
	}

	void blDX12::DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData)
	{
		mCompositeMeshRenderComponent->Render(renderData);
	}

	void blDX12::DrawCompositeMeshWithPass(const RenderCompositeMeshDataWithPassConstants& renderData)
	{
		mCompositeMeshRenderWithPassConstantsRenderComponent->Render(renderData);
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
		mGlobalRenderData.commandQueue->Signal(
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