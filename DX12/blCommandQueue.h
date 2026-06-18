#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include <memory>
#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandQueue
	{
	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		std::shared_ptr<blDevice> mDevice;
	public:
		blCommandQueue(std::shared_ptr<blDevice> device, std::wstring name = L"");

		ComPtr<ID3D12CommandQueue> GetDX12CommandQueue()
		{
			return mCommandQueue;
		}

		std::shared_ptr<blDevice> GetDevice()
		{
			return mDevice;
		}
	};
}