#include "blCommandList.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandList::blCommandList(blCommandListAllocator* commandListAllocator, std::wstring name)
		: mCommandListAllocator(commandListAllocator)
	{
		DX12_API_CALL(commandListAllocator->GetDevice()->GetDX12Device()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList)));

		DX12_API_CALL(mCommandList->SetName((L"[BL] " + name).c_str()));
	}

	void blCommandList::Close() const
	{
		mCommandList->Close();
	}

	void blCommandList::Reset()
	{
		mCommandList->Reset(mCommandListAllocator->GetAllocatorPtr().Get(), nullptr);
	}

	void blCommandList::Reset(blCommandListAllocator* commandListAllocator)
	{
		mCommandListAllocator = commandListAllocator;
		Reset();
	}
}