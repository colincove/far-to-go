#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include <memory>
#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blFence
	{
	private:
		ComPtr<ID3D12Fence> mFence;
	public:
		blFence(std::shared_ptr<blDevice> device);
		ComPtr<ID3D12Fence> GetFence() { return mFence; }
	};
}