#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandListAllocator
	{
	private:
		ComPtr<ID3D12CommandAllocator> mCommandListAllocator;
		blDevice* mDevice;
	public:
		blCommandListAllocator(blDevice* device, std::wstring name = L"");

		const ComPtr<ID3D12CommandAllocator> GetAllocatorPtr() const
		{
			return mCommandListAllocator;
		}

		blDevice* GetDevice()
		{
			return mDevice;
		}

		void Reset();
	};
}