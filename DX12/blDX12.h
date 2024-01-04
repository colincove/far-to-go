#pragma once

//BoulderLeaf
#include "blDX12Core.h"
#include "../Graphics/blGraphicsAPI.h"
#include <dxgi.h>
#include <d3d12.h>

#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	//NOTE: This format was randomly chosen. Verify correct value later
	constexpr DXGI_FORMAT sBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;;

	class DX12 : public API
	{
	private:
		ComPtr<IDXGIFactory1> mFactory;
		ComPtr<ID3D12Device8> mDevice;
		ComPtr<ID3D12Fence> mFence;
		UINT mRvtDescriptorSize;
		UINT mDsvDescriptorSize;
		UINT mCbvSrvDescriptorSize;
	public:
		static std::weak_ptr<DX12> Get();
		DX12();
		void Initialize();
	};
}