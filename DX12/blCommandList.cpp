#include "blCommandList.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandList::blCommandList(
		ComPtr<ID3D12Device8> device,
		std::shared_ptr<blCommandListAllocator> commandListAllocator)
	{
		DX12_API_CALL(device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList)));
	}
}