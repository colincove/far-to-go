#pragma once

#include <blSwapChain.h>
#include <directx/d3dx12.h>

namespace BoulderLeaf::Graphics::DX12
{
	blSwapChain::blSwapChain(
		size_t bufferCount,
		const std::shared_ptr<blDevice> device) : 
		mSwapChainBuffers(bufferCount),
		mBufferCount(bufferCount)
	{
		D3D12_DESCRIPTOR_HEAP_DESC rvtHeapDesc;
		rvtHeapDesc.NumDescriptors = bufferCount;
		rvtHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rvtHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rvtHeapDesc.NodeMask = 0;

		mRtvDescriptorSize = device->GetDX12Device()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		DX12_API_CALL(device->GetDX12Device()->CreateDescriptorHeap(&rvtHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart());

		for (UINT i = 0; i < bufferCount; i++)
		{
			// Get the ith buffer in the swap chain.
			DX12_API_CALL(mSwapChain->GetBuffer(
				i, IID_PPV_ARGS(&mSwapChainBuffers[i])));
			// Create an RTV to it.
			device->GetDX12Device()->CreateRenderTargetView(mSwapChainBuffers[i].Get(), nullptr, rtvHeapHandle);
			mSwapChainBuffers[i]->SetName(L"SwapChainBuffer");

			// Next entry in heap.
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
		}
	}

	void blSwapChain::Present()
	{
		DX12_API_CALL(mSwapChain->Present(0, 0));
		mCurrBackBuffer = (mCurrBackBuffer + 1) % mBufferCount;
	}
}