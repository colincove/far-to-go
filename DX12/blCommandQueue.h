#pragma once

#include <blDX12Core.h>
#include <d3d12.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandQueue
	{
	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
	public:
		blCommandQueue(ComPtr<ID3D12Device8> device);

		ComPtr<ID3D12CommandQueue> GetDX12CommandQueue()
		{
			return mCommandQueue;
		}
	};
}