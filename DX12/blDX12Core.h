#pragma once

//I dont understand why, but without this, CreateDXGIFactory could not link. 
#pragma comment(lib, "dxgi")

#include <wrl.h>

namespace BoulderLeaf::Graphics::DX12
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}