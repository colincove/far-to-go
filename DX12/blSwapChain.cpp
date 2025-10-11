#pragma once

#include <blSwapChain.h>

namespace BoulderLeaf::Graphics::DX12
{
	blSwapChain::blSwapChain(
		const ComPtr<ID3D12Device8> device,
		const ComPtr<ID3D12CommandQueue> commandQueue,
		const ComPtr<IDXGIFactory2> factory,
		const DXGI_FORMAT backbufferFormat,
		HWND& mainOutputWindow)
	{

	}
}