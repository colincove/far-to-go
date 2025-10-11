#pragma once

#include <blDX12Core.h>
#include "dxgi1_2.h"

namespace BoulderLeaf::Graphics::DX12
{
	class blFactory
	{
	private:
		ComPtr<IDXGIFactory2> mFactory;
	public:
		blFactory();
		ComPtr<IDXGIFactory2> GetDX12Factory();
	};
}