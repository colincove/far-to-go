#pragma once

#include <blDX12Core.h>
#include <d3d12.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blFence
	{
	private:
		ComPtr<ID3D12Fence> mFence;
	public:
		blFence(ComPtr<ID3D12Device8> device);
	};
}