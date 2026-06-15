#include "blCommandList.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandList::blCommandList(std::shared_ptr<blCommandListAllocator> commandListAllocator)
		: mCommandListAllocator(commandListAllocator)
	{
		HRESULT result = commandListAllocator->GetDevice()->GetDX12Device()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList));
		int test = 0;
		switch (result)
		{
			case S_OK:
				test = 43;
				break;
			case D3D12_ERROR_ADAPTER_NOT_FOUND:
				test = 74;
				break;
			case D3D12_ERROR_DRIVER_VERSION_MISMATCH:
				test = 65;
				break;
			case E_OUTOFMEMORY:
				test = 96;

				break;
			case E_INVALIDARG:
				test = 78;
				break;
			case E_FAIL:
				test = 86;
				break;
			case E_NOTIMPL:
				test = 32;
				break;
			case E_POINTER:
				test = 97;
				break;
		}

		assert(result == S_OK);

		/*DX12_API_CALL(commandListAllocator->GetDevice()->GetDX12Device()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList)));*/
	}
}