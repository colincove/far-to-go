#include "blCommandListAllocator.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandListAllocator::blCommandListAllocator(blDevice* device, std::wstring name)
		: mDevice(device)
	{
		DX12_API_CALL(device->GetDX12Device()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&mCommandListAllocator)));

		DX12_API_CALL(mCommandListAllocator->SetName((L"[BL] " + name).c_str()));
	}

	void blCommandListAllocator::Reset()
	{
		HRESULT result = mCommandListAllocator->Reset();

		if (FAILED(result))
		{
			bool wtf = false;
		}
	}
}