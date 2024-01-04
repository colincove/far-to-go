//DirectX
#include <directx/d3d12.h>
#include <directx/d3d12video.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>
#include "dxguids/dxguids.h"
#include "directx/d3d12sdklayers.h"
#include "dxgi1_3.h"
#include <dxgidebug.h>

//BoulderLeaf
#include "blDX12Test.h"
#include "blDX12.h"

//Standard
#include <iostream>
#include <atlstr.h>
#include <assert.h>

namespace BoulderLeaf::Graphics::DX12
{
	namespace
	{
		UINT GetM4xMsaaQuality(const DXGI_FORMAT backbufferFormat, const ComPtr<ID3D12Device8> device);
	}

	std::weak_ptr<DX12> DX12::Get()
	{
		static std::shared_ptr<DX12> sDx12;

		if (sDx12 == nullptr)
		{
			sDx12 = std::shared_ptr<DX12>(new DX12());
		}

		return sDx12;
	}

	DX12::DX12() :
		mFactory(nullptr), 
		mDevice(nullptr),
		mFence(nullptr),
		mRvtDescriptorSize(0),
		mDsvDescriptorSize(0),
		mCbvSrvDescriptorSize(0)
	{

	}

	void DX12::Initialize()
	{

		DWORD dxgiFactoryFlags = 0;
#ifdef DEBUG
		ComPtr<ID3D12Debug> debugController;
		assert((D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)) == S_OK));
		debugController->EnableDebugLayer();

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}
#endif // DEBUG

		HRESULT factoryResult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory));
		assert((factoryResult == S_OK));

		HRESULT deviceResult = D3D12CreateDevice(
			nullptr, // default adapter
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&mDevice));
		assert((deviceResult == S_OK));

		HRESULT fenceResult = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
		assert((fenceResult == S_OK));

		mRvtDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvSrvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		assert((GetM4xMsaaQuality(sBackbufferFormat, mDevice) > 0));
	}

	namespace
	{
		UINT GetM4xMsaaQuality(const DXGI_FORMAT backbufferFormat, const ComPtr<ID3D12Device8> device)
		{
			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
			msQualityLevels.Format = backbufferFormat;
			msQualityLevels.SampleCount = 4;
			msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			msQualityLevels.NumQualityLevels = 0;

			HRESULT result = device->CheckFeatureSupport(
				D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&msQualityLevels,
				sizeof(msQualityLevels)
			);

			assert((result == S_OK));

			return msQualityLevels.NumQualityLevels;
		}
	}
}

