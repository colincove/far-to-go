#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include <memory>
#include <blDevice.h>
#include <string>

namespace BoulderLeaf::Graphics::DX12
{
	class blFence
	{
	private:
		ComPtr<ID3D12Fence> mFence;
	public:
		blFence(blDevice* device, std::wstring name = L"Default");
		ID3D12Fence* Get() { return mFence.Get(); }

		UINT64 GetCompletedValue();

		HRESULT SetEventOnCompletion(
			UINT64 Value,
			HANDLE hEvent);

		HRESULT Signal(
			UINT64 Value);

		template<typename... Args>
		HRESULT SetEventOnCompletion(Args&&... args)
		{
			return mFence->SetEventOnCompletion(std::forward<Args>(args)...);
		}
	};
}