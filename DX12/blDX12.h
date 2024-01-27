#pragma once

//BoulderLeaf
#include "blDX12Core.h"
#include "../Graphics/blGraphicsAPI.h"
#include <dxgi.h>
#include <d3d12.h>
#include <dxgi1_2.h>

#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	//NOTE: This format was randomly chosen. Verify correct value later
	constexpr DXGI_FORMAT sBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;;

	class DX12 : public API
	{
	private:
		ComPtr<IDXGIFactory2> mFactory;
		ComPtr<ID3D12Device8> mDevice;
		ComPtr<ID3D12Fence> mFence;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mDirectCommandListAllocator;
		ComPtr<ID3D12CommandList> mCommandList;
		ComPtr<IDXGISwapChain1> mSwapChain;
		UINT mRvtDescriptorSize;
		UINT mDsvDescriptorSize;
		UINT mCbvSrvDescriptorSize;
		UINT mClientWidth;
		UINT mClientHeight;

		//application
		HWND mhMainWnd;
	public:
		static std::weak_ptr<DX12> Get();
		DX12();
		void Initialize();
	};
}