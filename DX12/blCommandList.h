#pragma once

#include <blDX12Core.h>
#include <blCommandListAllocator.h>
#include <d3d12.h>
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandList
	{
	private:
		ComPtr<ID3D12Fence> mCommandList;
	public:
		blCommandList(
			ComPtr<ID3D12Device8> device,
			std::shared_ptr<blCommandListAllocator> commandListAllocator);
	};
}