#include "blDepthBuffer.h"

#include <directx/d3d12.h>
#include <directx/d3d12video.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDepthBuffer::blDepthBuffer(
		const std::shared_ptr<blDevice> device, 
		const std::shared_ptr<Core::blWindow> window)
	{
		D3D12_RESOURCE_DESC desc;

		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = window->GetWidth();
		desc.Height = window->GetHeight();
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = Constants::DepthStencilFormat;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = Constants::DepthStencilFormat;
		optClear.DepthStencil.Depth = 1;
		optClear.DepthStencil.Stencil = 0;

		CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		DX12_API_CALL(device->GetDX12Device()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&optClear,
			IID_PPV_ARGS(mDepthBufferView.GetAddressOf())));

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;

		DX12_API_CALL(device->GetDX12Device()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));

		// Create descriptor to mip level 0 of entire resource using the
		// format of the resource.
		device->GetDX12Device()->CreateDepthStencilView(
			mDepthBufferView.Get(),
			nullptr,
			DepthStencilView());

		DX12_API_CALL(mDepthBufferView->SetName(L"[BL] DepthBuffer"));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE blDepthBuffer::DepthStencilView() const
	{
		return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}
}