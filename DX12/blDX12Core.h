#pragma once

//I dont understand why, but without this, CreateDXGIFactory could not link. 
#pragma comment(lib, "dxgi")

#include <wrl.h>
#include <assert.h>
#include <atldef.h>

namespace BoulderLeaf::Graphics::DX12
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

#ifdef DEBUG
	#define DX12_API_CALL(a) assert((a) == S_OK)
#else
	#define DX12_API_CALL(a) (a)
#endif // DEBUG

#ifdef DEBUG
	#define DX12_API_CALL_SUCCEEDED(a) ((a) == S_OK)
#else
	#define DX12_API_CALL_SUCCEEDED(a) (a)
#endif // DEBUG
}