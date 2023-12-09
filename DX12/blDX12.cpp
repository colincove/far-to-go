//DirectX
#include <directx/d3d12.h>
#include <directx/d3d12video.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>
#include "dxguids/dxguids.h"
#include "directx/d3d12sdklayers.h"

//BoulderLeaf
#include "blDX12Test.h"
#include "blDX12.h"

//Standard
#include <iostream>
#include <atlstr.h>
#include <assert.h>

namespace BoulderLeaf::Graphics::DX12
{
	std::weak_ptr<DX12> DX12::Get()
	{
		static std::shared_ptr<DX12> sDx12Control;

		if (sDx12Control == nullptr)
		{
			sDx12Control = std::shared_ptr<DX12>(new DX12());
		}

		return sDx12Control;
	}

	DX12::DX12() :
		mFactory(nullptr), 
		mDevice(nullptr)
	{

	}

	void DX12::Initialize()
	{
#ifdef DEBUG
		{
			//TODO: ID3D12GetDebugInterface will not resolve for some reason. but is is part of the d3d12 header. Not sure why it's broken
			//ComPtr<ID3D12Debug> debugController;
			//assert((ID3D12GetDebugInterface(IID_PPV_ARGS(&debugController)) == S_OK));
			//debugController->EnableDebugLayer();
		}
#endif // DEBUG
		HRESULT factoryResult = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));
		assert((factoryResult == S_OK));

		HRESULT deviceResult = D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&mDevice));
		assert((deviceResult == S_OK));
	}
}