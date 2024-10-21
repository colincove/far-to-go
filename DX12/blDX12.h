#pragma once

//BoulderLeaf
#include "blDX12Core.h"
#include "../Graphics/blGraphicsAPI.h"
#include <dxgi.h>
#include <d3d12.h>
#include <dxgi1_2.h>

#include <memory>
#include <vector>

namespace BoulderLeaf::Graphics::DX12
{
	//NOTE: This format was randomly chosen. Verify correct value later
	constexpr DXGI_FORMAT sBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	constexpr DXGI_FORMAT sDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	constexpr int SwapChainBufferCount = 2;

	class DX12 : public API
	{
	public:
		ComPtr<IDXGIFactory2> mFactory;
		ComPtr<ID3D12Device8> mDevice;
		ComPtr<ID3D12Fence> mFence;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mDirectCommandListAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
		ComPtr<IDXGISwapChain1> mSwapChain;
		ComPtr<ID3D12DescriptorHeap> mRtvHeap;
		ComPtr<ID3D12DescriptorHeap> mDsvHeap;
		ComPtr<ID3D12Resource> mDepthBufferView;
		UINT mRtvDescriptorSize;
		UINT mDsvDescriptorSize;
		UINT mCbvSrvDescriptorSize;
		UINT mClientWidth;
		UINT mClientHeight;
		bool m_isInitialized;
		
		//application
		HWND mhMainWnd;
		int mCurrBackBuffer;

	public:
		static std::weak_ptr<DX12> Get();
		DX12();
		void Initialize();
		bool IsInitialized() const;
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
		std::vector<ComPtr<ID3D12Resource>> CreateRenderTargetViewToBackBuffer() const;
		void CreateDepthBufferView(
			const int width,
			const int height,
			const ComPtr<ID3D12Device8> device,
			ComPtr<ID3D12Resource>& depthBufferViewOut) const;
	};
}