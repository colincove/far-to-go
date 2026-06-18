#pragma once

#include <blDX12Core.h>
#include <blDevice.h>
#include <blCommandListAllocator.h>
#include <d3d12.h>
#include <memory>
#include <blPSO.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandList
	{
	private:
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
		std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
	public:
		blCommandList(std::shared_ptr<blCommandListAllocator> commandListAllocator, std::wstring name = L"");
		ComPtr<ID3D12GraphicsCommandList>& GetCommandListPtr() { return mCommandList; }
	};
}