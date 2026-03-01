#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>
#include <blDevice.h>
#include <blWindow.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blConstantBufferDescriptorHeap
	{
	private:
		ComPtr<ID3D12Resource> mDepthBufferView;
		ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
	public:
		blConstantBufferDescriptorHeap(
			const std::shared_ptr<blDevice> device);

		ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return mDescriptorHeap; };
	};
}