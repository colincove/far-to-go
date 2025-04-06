#pragma once

//BoulderLeaf
#include <assert.h>
#include "blDX12Core.h"
#include "../Graphics/blGraphicsAPI.h"
#include <dxgi.h>
#include <d3d12.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>

#include <memory>
#include <vector>

namespace BoulderLeaf::Graphics::DX12
{
	using namespace DirectX;

	XMFLOAT4X4 XMFLOAT4X4_Identity();
	

	//NOTE: This format was randomly chosen. Verify correct value later
	constexpr DXGI_FORMAT sBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	constexpr DXGI_FORMAT sDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	constexpr int SwapChainBufferCount = 2;
	constexpr int sSrvHeapSize = 64;

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
		ComPtr<ID3D12DescriptorHeap> mCbvHeap;
		ComPtr<ID3D12Resource> mDepthBufferView;
		UINT mRtvDescriptorSize;
		UINT mDsvDescriptorSize;
		UINT mCbvSrvDescriptorSize;
		bool m_isInitialized;
		
		//application
		HWND mhMainWnd;
		int mCurrBackBuffer;
		int m4xMsaaQuality;
		bool m4xMsaaState;
		UINT64 mCurrentFence;
		D3D12_VIEWPORT mViewPort;
		D3D12_RECT mScissorRect;
		ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
		ComPtr<ID3D12Resource> mDepthStencilBuffer;
		XMFLOAT4X4 mProj;
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	public:
		static std::weak_ptr<DX12> Get();
		DX12();
		void Initialize(HWND mhMainWnd);
		bool IsInitialized() const;
		void FlushCommandQueue();
		ID3D12Resource* CurrentBackBuffer() const;
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferView(int i) const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

		float AspectRatio() const
		{
			RECT winRect;
			assert(GetWindowRect(mhMainWnd, &winRect));
			const int width = winRect.right - winRect.left;
			const int height = winRect.bottom - winRect.top;
			
			return static_cast<float>(width) / height;
		}

	private:
		std::vector<ComPtr<ID3D12Resource>> CreateRenderTargetViewToBackBuffer() const;
		void CreateDepthBufferView(
			const RECT winRect,
			const ComPtr<ID3D12Device8> device,
			ComPtr<ID3D12Resource>& depthBufferViewOut) const;
		void PopulateSwapChainBuffers();
		void BuildDescriptorHeaps();
	};
}