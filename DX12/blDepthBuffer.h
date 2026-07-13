#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>
#include <blDevice.h>
#include <blWindow.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDepthBuffer
	{
	private:
		ComPtr<ID3D12Resource> mDepthBufferView;
		ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	public:
		blDepthBuffer(
			blDevice* device,
			const Core::blWindow* window);

		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	};
}