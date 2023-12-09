#pragma once

//BoulderLeaf
#include "blDX12Core.h"
#include <dxgi.h>
#include <d3d12.h>

#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class DX12
	{
	private:
		ComPtr<IDXGIFactory1> mFactory;
		ComPtr<ID3D12Device8> mDevice;
	public:

		static std::weak_ptr<DX12> Get();

		DX12();
		void Initialize();
	};
}