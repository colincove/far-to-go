#pragma once

#include <blDX12Core.h>
#include <d3d12.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandListAllocator
	{
	private:
		ComPtr<ID3D12CommandAllocator> mCommandListAllocator;
	public:
		blCommandListAllocator(ComPtr<ID3D12Device8> device);

		const ComPtr<ID3D12CommandAllocator> GetAllocatorPtr() const
		{
			return mCommandListAllocator;
		}
	};
}