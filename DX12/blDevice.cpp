#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDevice::blDevice()
	{
		DX12_API_CALL(D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&mDevice)));

		mCommandListAllocator = std::make_shared<blCommandListAllocator>(mDevice);

		mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	std::shared_ptr<blFence> blDevice::CreateFence() const
	{
		return std::make_shared<blFence>(mDevice);
	}

	std::shared_ptr<blCommandQueue> blDevice::CreateCommandQueue() const
	{
		return std::make_shared<blCommandQueue>(mDevice);
	}

	std::shared_ptr<blCommandList> blDevice::CreateCommandList() const
	{
		return std::make_shared<blCommandList>(mDevice, mCommandListAllocator);
	}
}