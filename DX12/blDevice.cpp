#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDevice::blDevice(std::wstring name)
	{
		DX12_API_CALL(D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&mDevice)));

		mDevice->SetName((L"[BL] " + name).c_str());

		mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
}