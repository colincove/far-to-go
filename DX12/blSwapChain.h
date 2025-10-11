#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class blSwapChain
	{
	private:
		ComPtr<ID3D12Fence> mSwapChain;
	public:
		blSwapChain(
			const ComPtr<ID3D12Device8> device,
			const ComPtr<ID3D12CommandQueue> commandQueue,
			const ComPtr<IDXGIFactory2> factory,
			const DXGI_FORMAT backbufferFormat,
			HWND& mainOutputWindow);
	};
}