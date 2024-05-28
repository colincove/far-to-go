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

//Standard
#include <iostream>
#include <atlstr.h>
#include <assert.h>

namespace BoulderLeaf::Graphics::DX12
{
	namespace
	{
		UINT GetM4xMsaaQuality(const DXGI_FORMAT backbufferFormat, const ComPtr<ID3D12Device8> device);
		HRESULT CreateSwapChain(
			const ComPtr<ID3D12Device8> device,
			const ComPtr<ID3D12CommandQueue> commandQueue,
			const ComPtr<IDXGIFactory2> factory,
			const UINT width,
			const UINT height,
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
		mRtvDescriptorSize(0),
		mDsvDescriptorSize(0),
		mCbvSrvDescriptorSize(0),
		mClientWidth(1024),
		mClientHeight(768),
		mhMainWnd(nullptr),
		mCurrBackBuffer(0)
	{

	}

	void DX12::Initialize()
	{
		mhMainWnd = ::CreateWindowA("STATIC", "dummy", WS_VISIBLE, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
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

		HRESULT swapChainResult = CreateSwapChain(
			mDevice, 
			mCommandQueue, 
			mFactory,
			mClientWidth, 
			mClientHeight, 
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

		assert((descriptorHeapsResult == S_OK));

		std::vector<ComPtr<ID3D12Resource>> backBufferRenderTargetView = CreateRenderTargetViewToBackBuffer();

		CreateDepthBufferView(
			mClientWidth,
			mClientHeight,
			mDevice,
			mDepthBufferView);

		//---------- create viewport -----------//
		D3D12_VIEWPORT vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = static_cast<float>(mClientWidth);
		vp.Height = static_cast<float>(mClientHeight);
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		mCommandList->RSSetViewports(1, &vp);
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
		}

		return swapChainBuffer;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12::CurrentBackBufferView() const
	{	
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrBackBuffer,
			mRtvDescriptorSize
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12::DepthStencilView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mDsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void DX12::CreateDepthBufferView(
		const int width, 
		const int height,
		const ComPtr<ID3D12Device8> device,
		ComPtr<ID3D12Resource>& depthBufferViewOut) const
	{
		D3D12_RESOURCE_DESC desc;

		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = width;
		desc.Height = height;
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
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(depthBufferViewOut.GetAddressOf()));

		assert(result == S_OK);

		// Create descriptor to mip level 0 of entire resource using the
		// format of the resource.
		device->CreateDepthStencilView(
			depthBufferViewOut.Get(),
			nullptr,
			DepthStencilView());

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			depthBufferViewOut.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE);

		// Transition the resource from its initial state to be used as a depth buffer.
		mCommandList->ResourceBarrier(
			1,
			&barrier);


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
			const UINT width,
			const UINT height,
			const DXGI_FORMAT backbufferFormat,
			HWND& mainOutputWindow,
			ComPtr<IDXGISwapChain1>& swapChainOut)
		{
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