#pragma once

#include <blCommandQueue.h>
#include <blCommandList.h>
#include <blFence.h>
#include <blDX12Core.h>
#include <blCommandListAllocator.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class blDevice
	{
	private:
		ComPtr<ID3D12Device8> mDevice;
		std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
		UINT mRtvDescriptorSize;
		UINT mDsvDescriptorSize;
		UINT mCbvSrvDescriptorSize;
	public:
		blDevice();
	public:
		std::shared_ptr<blFence> CreateFence() const;
		std::shared_ptr<blCommandQueue> CreateCommandQueue() const;
		std::shared_ptr<blCommandList> CreateCommandList() const;

		ComPtr<ID3D12Device8> GetDX12Device()
		{
			return mDevice;
		}

		std::shared_ptr<blCommandListAllocator> GetCommandListAllocator()
		{
			return mCommandListAllocator;
		}
	};
}