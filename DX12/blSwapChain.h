#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include "dxgi1_2.h"
#include <memory>
#include <blWindow.h>
#include <blDevice.h>
#include <blCommandQueue.h>
#include <blFactory.h>
#include <vector>
#include <dxgi1_3.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blSwapChain
	{
	private:
		size_t mBufferCount;
		ComPtr<ID3D12DescriptorHeap> mRtvHeap;
		ComPtr<IDXGISwapChain2> mSwapChain;
		UINT mRtvDescriptorSize;
		HANDLE mSwapChainWaitableObject;
		std::vector<ComPtr<ID3D12Resource>> mSwapChainBuffers;
		int mCurrBackBuffer;
	public:
		blSwapChain(
			size_t bufferCount,
			const std::shared_ptr<blDevice> device,
			const std::shared_ptr<Core::blWindow> window,
			const std::shared_ptr<blCommandQueue> commandQueue,
			const std::shared_ptr<blFactory> factory);

		void Present();
		ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const { return mRtvHeap; }
		ComPtr<IDXGISwapChain2> GetSwapChain() const { return mSwapChain; }
		int GetCurrentBackBufferIndex() const { return mCurrBackBuffer; }
		ID3D12Resource* GetCurrentBackBuffer() const { return mSwapChainBuffers[mCurrBackBuffer].Get(); }
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	};
}