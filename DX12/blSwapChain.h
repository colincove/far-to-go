#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>
#include <blWindow.h>
#include <blDevice.h>
#include <blCommandQueue.h>
#include <blFactory.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blSwapChain
	{
	private:
		ComPtr<ID3D12Fence> mSwapChain;
	public:
		blSwapChain(
			const std::shared_ptr<blDevice> device,
			const std::shared_ptr<blCommandQueue> commandQueue,
			const std::shared_ptr<blFactory> factory,
			const std::shared_ptr<Core::blWindow> window);
	};
}