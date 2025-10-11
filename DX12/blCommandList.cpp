#include "blCommandList.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandList::blCommandList(std::shared_ptr<blCommandListAllocator> commandListAllocator)
	{
		DX12_API_CALL(commandListAllocator->GetDevice()->GetDX12Device()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList)));
	}
}