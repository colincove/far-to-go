#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class blDevice
	{
	private:
		ComPtr<ID3D12Device8> mDevice;

		UINT mRtvDescriptorSize;
		UINT mDsvDescriptorSize;
		UINT mCbvSrvDescriptorSize;
	public:
		blDevice(std::wstring name = L"Default");
	public:

		ComPtr<ID3D12Device8> GetDX12Device()
		{
			return mDevice;
		}

		UINT GetRtvDescriptorSize() const
		{
			return mRtvDescriptorSize;
		}

		UINT GetDsvDescriptorSize() const
		{
			return mDsvDescriptorSize;
		}

		UINT GetCbvSrvDescriptorSize() const
		{
			return mCbvSrvDescriptorSize;
		}
	};
}