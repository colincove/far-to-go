#pragma once

#include <blSwapChain.h>
#include <directx/d3dx12.h>
#include <format>

namespace
{
	using namespace BoulderLeaf::Graphics::DX12;
	UINT GetM4xMsaaQuality(const DXGI_FORMAT backbufferFormat, const ComPtr<ID3D12Device8> device);
}

namespace BoulderLeaf::Graphics::DX12
{
	blSwapChain::blSwapChain(
		size_t bufferCount,
		const std::shared_ptr<blDevice> device,
		const std::shared_ptr<Core::blWindow> window,
		const std::shared_ptr<blCommandQueue> commandQueue,
		const std::shared_ptr<blFactory> factory) :
		mSwapChainBuffers(bufferCount),
		mBufferCount(bufferCount),
		mCurrBackBuffer(0)
	{
		RECT winRect;
		assert(GetWindowRect(window->GetWindowHandle(), &winRect));
		const int width = winRect.right - winRect.left;
		const int height = winRect.bottom - winRect.top;

		mSwapChain.Reset();

		DXGI_SWAP_CHAIN_DESC1 desc;
		const int swapChainBufferCount = 2;

		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Width = width;
		desc.Height = height;
		desc.Stereo = FALSE;
		desc.Format = Constants::BackbufferFormat;
		desc.Scaling = DXGI_SCALING_NONE;

		auto& sampleDesc = desc.SampleDesc;
		const int m4xMsaaQuality = GetM4xMsaaQuality(Constants::BackbufferFormat, device->GetDX12Device());
		const bool m4xMsaaState = m4xMsaaQuality > 0;
		//DXGI ERROR: IDXGIFactory::CreateSwapChain: Flip model swapchains (DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL and DXGI_SWAP_EFFECT_FLIP_DISCARD) do not support multisampling. DXGI_SWAP_CHAIN_DESC{ SwapChainType = ..._HWND, BufferDesc = DXGI_MODE_DESC1{Width = 1024, Height = 768, RefreshRate = DXGI_RATIONAL{ Numerator = 60, Denominator = 1 }, Format = R8G8B8A8_UNORM, ScanlineOrdering = ..._UNSPECIFIED, Scaling = (Unknown: 0xcccccccc), Stereo = FALSE }, SampleDesc = DXGI_SAMPLE_DESC{ Count = 4, Quality = 0 }, BufferUsage = 0x20, BufferCount = 2, OutputWindow = 0x0000000000730AFA, Scaling = ..._NONE, Windowed = TRUE, SwapEffect = ..._FLIP_DISCARD, AlphaMode = ..._UNSPECIFIED, Flags = 0x2 } [ MISCELLANEOUS ERROR #102: ]
		//sampleDesc.Count = m4xMsaaState ? 4 : 1;
		//sampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = swapChainBufferCount;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT was added so that 
		//GetFrameLatencyWaitableObject would actually return something
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
		fullscreenDesc.RefreshRate.Numerator = 60;
		fullscreenDesc.RefreshRate.Denominator = 1;
		fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fullscreenDesc.Windowed = true;

		//What is this nonsense? I searched high and low, and no factory interface can straight up create a IDXGISwapChain2 class. 
		//You need to do this goofy looking QueryInterface thing. It is very odd. WHy?
		//(code taken from imgui dx12 backend examples)
		IDXGISwapChain1* swapChain1 = nullptr;
		factory->GetDX12Factory()->CreateSwapChainForHwnd(commandQueue->GetDX12CommandQueue().Get(), window->GetWindowHandle(), &desc, &fullscreenDesc, NULL, &swapChain1);
		DX12_API_CALL(swapChain1->QueryInterface(IID_PPV_ARGS(&mSwapChain)));
		swapChain1->Release();

		D3D12_DESCRIPTOR_HEAP_DESC rvtHeapDesc;
		rvtHeapDesc.NumDescriptors = bufferCount;
		rvtHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rvtHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rvtHeapDesc.NodeMask = 0;

		mRtvDescriptorSize = device->GetDX12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		DX12_API_CALL(device->GetDX12Device()->CreateDescriptorHeap(&rvtHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT i = 0; i < bufferCount; i++)
		{
			// Get the ith buffer in the swap chain.
			DX12_API_CALL(mSwapChain->GetBuffer(
				i, IID_PPV_ARGS(&mSwapChainBuffers[i])));
			// Create an RTV to it.
			device->GetDX12Device()->CreateRenderTargetView(mSwapChainBuffers[i].Get(), nullptr, rtvHeapHandle);
			mSwapChainBuffers[i]->SetName(std::format(L"[BL]SwapChainBuffer({})", i).c_str());
			// Next entry in heap.
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
		}
	}

	void blSwapChain::Present()
	{
		DX12_API_CALL(mSwapChain->Present(0, 0));
		mCurrBackBuffer = (mCurrBackBuffer + 1) % mBufferCount;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE blSwapChain::CurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrBackBuffer,
			mRtvDescriptorSize
		);
	}
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
}