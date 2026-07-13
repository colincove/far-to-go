#include <blFence.h>
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blFence::blFence(blDevice* device, std::wstring name)
	{
		DX12_API_CALL(device->GetDX12Device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
		DX12_API_CALL(mFence->SetName((L"[BL] "+ name).c_str()));
	}

	UINT64 blFence::GetCompletedValue()
	{
		return mFence->GetCompletedValue();
	}

	HRESULT blFence::SetEventOnCompletion(
		UINT64 Value,
		HANDLE hEvent)
	{
		return mFence->SetEventOnCompletion(Value, hEvent);
	}

	HRESULT blFence::Signal(
		UINT64 Value)
	{
		return mFence->Signal(Value);
	}
}