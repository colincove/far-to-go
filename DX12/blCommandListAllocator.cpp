#include "blCommandListAllocator.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandListAllocator::blCommandListAllocator(ComPtr<ID3D12Device8> device)
	{
		DX12_API_CALL(device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&mCommandListAllocator)));
	}
}