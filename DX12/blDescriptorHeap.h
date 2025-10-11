#pragma once

#include <blDX12Core.h>
#include <d3d12.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDescriptorHeap
	{
	private:
		ComPtr<ID3D12Fence> mDescriptorHeap;
	public:
		blDescriptorHeap(ComPtr<ID3D12Device8> device);
	};
}