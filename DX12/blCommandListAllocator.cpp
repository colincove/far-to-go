#include "blCommandListAllocator.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandListAllocator::blCommandListAllocator(std::shared_ptr<blDevice> device)
		: mDevice(device)
	{
		DX12_API_CALL(device->GetDX12Device()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&mCommandListAllocator)));
	}

	void blCommandListAllocator::Reset()
	{
		DX12_API_CALL(mCommandListAllocator->Reset());
	}
}