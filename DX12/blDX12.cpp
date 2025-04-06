//DirectX
#include <directx/d3d12.h>
#include <directx/d3d12video.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>
#include "dxguids/dxguids.h"
#include "directx/d3d12sdklayers.h"
#include "dxgi1_3.h"
#include <dxgidebug.h>

//BoulderLeaf
#include "blDX12Test.h"
#include "blDX12.h"
#include "..\Math\blMath.inl"

//Standard
#include <iostream>
#include <atlstr.h>
#include <assert.h>

namespace BoulderLeaf::Graphics::DX12
{
	XMFLOAT4X4 XMFLOAT4X4_Identity()
	{
		return XMFLOAT4X4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	namespace
	{
		UINT GetM4xMsaaQuality(const DXGI_FORMAT backbufferFormat, const ComPtr<ID3D12Device8> device);
		HRESULT CreateSwapChain(
			const ComPtr<ID3D12Device8> device,
			const ComPtr<ID3D12CommandQueue> commandQueue,
			const ComPtr<IDXGIFactory2> factory,
			const DXGI_FORMAT backbufferFormat,
			HWND& mainOutputWindow,
			ComPtr<IDXGISwapChain1>& swapChainOut);

		HRESULT CreateRtvAndDsvDescriptorHeaps(
			const ComPtr<ID3D12Device8> device,
			const int swapChainBufferCount,
			ComPtr<ID3D12DescriptorHeap>& mRtvHeapOut,
			ComPtr<ID3D12DescriptorHeap>& mDsvHeapOut);
	}

	std::weak_ptr<DX12> DX12::Get()
	{
		static std::shared_ptr<DX12> sDx12;

		if (sDx12 == nullptr)
		{
			sDx12 = std::shared_ptr<DX12>(new DX12());
		}

		return sDx12;
	}

	DX12::DX12() :
		mFactory(nullptr),
		mDevice(nullptr),
		mFence(nullptr),
		mCommandQueue(nullptr),
		mDirectCommandListAllocator(nullptr),
		mCommandList(nullptr),
		mSwapChain(nullptr),
		mRtvHeap(nullptr),
		mDsvHeap(nullptr),
		mCbvHeap(nullptr),
		mRtvDescriptorSize(0),
		mDsvDescriptorSize(0),
		mCbvSrvDescriptorSize(0),
		m_isInitialized(false),
		mhMainWnd(nullptr),
		mCurrBackBuffer(0),
		mCurrentFence(0),
		mViewPort(),
		mSwapChainBuffer(),
		mDepthStencilBuffer(nullptr),
		mProj(XMFLOAT4X4_Identity())
	{

	}

	void DX12::Initialize(HWND hMainWnd)
	{
		RECT winRect;
		assert(GetWindowRect(hMainWnd, &winRect));
		const int width = winRect.right - winRect.left;
		const int height = winRect.bottom - winRect.top;

		//mhMainWnd = ::CreateWindowA("STATIC", "dummy", WS_VISIBLE, 0, 0, mClientWidth, mClientHeight, NULL, NULL, NULL, NULL);
		mhMainWnd = hMainWnd;
		DWORD dxgiFactoryFlags = 0;
#ifdef DEBUG
		ComPtr<ID3D12Debug> debugController;
		assert((D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)) == S_OK));
		debugController->EnableDebugLayer();

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}
#endif // DEBUG

		HRESULT factoryResult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory));
		assert((factoryResult == S_OK));

		HRESULT deviceResult = D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&mDevice));
		assert((deviceResult == S_OK));

		HRESULT fenceResult = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
		assert((fenceResult == S_OK));

		mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		assert((GetM4xMsaaQuality(sBackbufferFormat, mDevice) > 0));

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		HRESULT commandQueueResult = mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));
		assert((commandQueueResult == S_OK));
		HRESULT commandAllocatorResult = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mDirectCommandListAllocator));
		assert((commandAllocatorResult == S_OK));
		HRESULT commandListResult = mDevice->CreateCommandList(
			0, 
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			mDirectCommandListAllocator.Get(), 
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList));
		assert((commandListResult == S_OK));
		mCommandList->SetName(L"BL- DX12 Command List");

		m4xMsaaQuality = GetM4xMsaaQuality(sBackbufferFormat, mDevice);
		m4xMsaaState = m4xMsaaQuality > 0;

		HRESULT swapChainResult = CreateSwapChain(
			mDevice, 
			mCommandQueue, 
			mFactory,
			sBackbufferFormat, 
			mhMainWnd,
			mSwapChain);

		assert((swapChainResult == S_OK));

		HRESULT descriptorHeapsResult = CreateRtvAndDsvDescriptorHeaps(
			mDevice,
			SwapChainBufferCount,
			mRtvHeap,
			mDsvHeap
		);

		//this was taken from the BoxExample. A 3rd kind of heap. What is the difference between the 3?
		BuildDescriptorHeaps();

		assert((descriptorHeapsResult == S_OK));

		PopulateSwapChainBuffers();

		std::vector<ComPtr<ID3D12Resource>> backBufferRenderTargetView = CreateRenderTargetViewToBackBuffer();

		CreateDepthBufferView(
			winRect,
			mDevice,
			mDepthBufferView);

		//---------- create viewport -----------//
		mViewPort.TopLeftX = 0;
		mViewPort.TopLeftY = 0;
		mViewPort.Width = static_cast<float>(width);
		mViewPort.Height = static_cast<float>(height);
		mViewPort.MinDepth = 0;
		mViewPort.MaxDepth = 1;
		mCommandList->RSSetViewports(1, &mViewPort);

		// The below rect definition would render everything in the top left corner of the screen. 
		// Use this to cull rendering from a portion of the back buffer. 
		// mScissorRect = { 0, 0, static_cast<long>(width) / 2, static_cast<long>(height) / 2 };

		// for now, we want the entire screen rendered
		mScissorRect = { 0, 0, static_cast<long>(width), static_cast<long>(height) };
		mCommandList->RSSetScissorRects(1, &mScissorRect);

		//finish
		m_isInitialized = true;
		OnResize();
	}

	void DX12::OnWindowMessage(MSG msg)
	{
	}

	void DX12::OnResize()
	{
		// The window resized, so update the aspect ratio and recompute the
			// projection matrix.
		XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * PIf,
			AspectRatio(), 1.0f, 1000.0f);
		XMStoreFloat4x4(&mProj, P);
	}

	std::vector<ComPtr<ID3D12Resource>> DX12::CreateRenderTargetViewToBackBuffer() const
	{
		std::vector<ComPtr<ID3D12Resource>> swapChainBuffer(SwapChainBufferCount);
		//swapChainBuffer.reserve(SwapChainBufferCount);
		
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT i = 0; i < SwapChainBufferCount; i++)
		{
			swapChainBuffer.push_back(nullptr);
			// Get the ith buffer in the swap chain.
			mSwapChain->GetBuffer(
				i, IID_PPV_ARGS(&swapChainBuffer[i]));
			// Create an RTV to it.
			mDevice->CreateRenderTargetView(
				swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			// Next entry in heap.
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
			swapChainBuffer[i]->SetName(L"BL - RenderTargetViewToBackBuffer");
		}

		return swapChainBuffer;
	}

	bool DX12::IsInitialized() const
	{
		return m_isInitialized;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12::CurrentBackBufferView() const
	{	
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrBackBuffer,
			mRtvDescriptorSize
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12::GetBackBufferView(int i) const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			i,
			mRtvDescriptorSize
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12::DepthStencilView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void DX12::CreateDepthBufferView(
		const RECT winRect,
		const ComPtr<ID3D12Device8> device,
		ComPtr<ID3D12Resource>& depthBufferViewOut) const
	{
		D3D12_RESOURCE_DESC desc;

		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = winRect.right - winRect.left;
		desc.Height = winRect.bottom - winRect.top;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = sDepthStencilFormat;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = sDepthStencilFormat;
		optClear.DepthStencil.Depth = 1;
		optClear.DepthStencil.Stencil = 0;

		CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		HRESULT result = device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&optClear,
			IID_PPV_ARGS(depthBufferViewOut.GetAddressOf()));

		assert(result == S_OK);

		// Create descriptor to mip level 0 of entire resource using the
		// format of the resource.
		device->CreateDepthStencilView(
			depthBufferViewOut.Get(),
			nullptr,
			DepthStencilView());

		depthBufferViewOut->SetName(L"BL - DepthBuffer");

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			depthBufferViewOut.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE);

		// Transition the resource from its initial state to be used as a depth buffer.
		mCommandList->ResourceBarrier(
			1,
			&barrier);
	}

	void DX12::FlushCommandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		mCurrentFence++;
		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to
		// this Signal().
		mCommandQueue->Signal(mFence.Get(), mCurrentFence);
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

	ID3D12Resource* DX12::CurrentBackBuffer() const
	{
		return mSwapChainBuffer[mCurrBackBuffer].Get();
	}

	void DX12::PopulateSwapChainBuffers()
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < SwapChainBufferCount; i++)
		{
			// Get the ith buffer in the swap chain.
			mSwapChain->GetBuffer(
				i, IID_PPV_ARGS(&mSwapChainBuffer[i]));
			// Create an RTV to it.
			mDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			mSwapChainBuffer[i]->SetName(L"SwapChainBuffer");
			// Next entry in heap.
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
		}
	}

	void DX12::BuildDescriptorHeaps()
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
		cbvHeapDesc.NumDescriptors = sSrvHeapSize;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;

		auto result = mDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap));
		assert((result == S_OK));
	}

	namespace
	{
		UINT GetM4xMsaaQuality(const DXGI_FORMAT backbufferFormat, const ComPtr<ID3D12Device8> device)
		{
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
			msQualityLevels.Format = backbufferFormat;
			msQualityLevels.SampleCount = 4;
			msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			msQualityLevels.NumQualityLevels = 0;

			HRESULT result = device->CheckFeatureSupport(
				D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&msQualityLevels,
				sizeof(msQualityLevels)
			);

			assert((result == S_OK));

			return msQualityLevels.NumQualityLevels;
		}

		HRESULT CreateSwapChain(
			const ComPtr<ID3D12Device8> device,
			const ComPtr<ID3D12CommandQueue> commandQueue,
			const ComPtr<IDXGIFactory2> factory,
			const DXGI_FORMAT backbufferFormat,
			HWND& mainOutputWindow,
			ComPtr<IDXGISwapChain1>& swapChainOut)
		{
			RECT winRect;
			assert(GetWindowRect(mainOutputWindow, &winRect));
			const int width = winRect.right - winRect.left;
			const int height = winRect.bottom - winRect.top;

			swapChainOut.Reset();

			DXGI_SWAP_CHAIN_DESC1 desc;
			const int swapChainBufferCount = 2;
			
			desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			desc.Width = width;
			desc.Height = height;
			desc.Stereo = FALSE;
			desc.Format = backbufferFormat;
			desc.Scaling = DXGI_SCALING_NONE;

			auto& sampleDesc = desc.SampleDesc;
			const int m4xMsaaQuality = GetM4xMsaaQuality(backbufferFormat, device);
			const bool m4xMsaaState = m4xMsaaQuality > 0;
			//DXGI ERROR: IDXGIFactory::CreateSwapChain: Flip model swapchains (DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL and DXGI_SWAP_EFFECT_FLIP_DISCARD) do not support multisampling. DXGI_SWAP_CHAIN_DESC{ SwapChainType = ..._HWND, BufferDesc = DXGI_MODE_DESC1{Width = 1024, Height = 768, RefreshRate = DXGI_RATIONAL{ Numerator = 60, Denominator = 1 }, Format = R8G8B8A8_UNORM, ScanlineOrdering = ..._UNSPECIFIED, Scaling = (Unknown: 0xcccccccc), Stereo = FALSE }, SampleDesc = DXGI_SAMPLE_DESC{ Count = 4, Quality = 0 }, BufferUsage = 0x20, BufferCount = 2, OutputWindow = 0x0000000000730AFA, Scaling = ..._NONE, Windowed = TRUE, SwapEffect = ..._FLIP_DISCARD, AlphaMode = ..._UNSPECIFIED, Flags = 0x2 } [ MISCELLANEOUS ERROR #102: ]
			//sampleDesc.Count = m4xMsaaState ? 4 : 1;
			//sampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
			sampleDesc.Count = 1;
			sampleDesc.Quality = 0;

			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = swapChainBufferCount;
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
			fullscreenDesc.RefreshRate.Numerator = 60;
			fullscreenDesc.RefreshRate.Denominator = 1;
			fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			fullscreenDesc.Windowed = true;

			return factory->CreateSwapChainForHwnd(commandQueue.Get(), mainOutputWindow , &desc, &fullscreenDesc, NULL, swapChainOut.GetAddressOf());
		}

		HRESULT CreateRtvAndDsvDescriptorHeaps(
			const ComPtr<ID3D12Device8> device,
			const int swapChainBufferCount,
			ComPtr<ID3D12DescriptorHeap>& rtvHeapOut,
			ComPtr<ID3D12DescriptorHeap>& dsvHeapOut)
		{
			D3D12_DESCRIPTOR_HEAP_DESC rvtHeapDesc;
			rvtHeapDesc.NumDescriptors = swapChainBufferCount;
			rvtHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rvtHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rvtHeapDesc.NodeMask = 0;

			HRESULT rvtResult = device->CreateDescriptorHeap(&rvtHeapDesc, IID_PPV_ARGS(rtvHeapOut.GetAddressOf()));
			assert(rvtResult == S_OK);

			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			dsvHeapDesc.NodeMask = 0;

			return device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(dsvHeapOut.GetAddressOf()));
		}
	}
}