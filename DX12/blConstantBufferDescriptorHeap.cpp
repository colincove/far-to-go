#include "blConstantBufferDescriptorHeap.h"

#include <directx/d3d12.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>


namespace BoulderLeaf::Graphics::DX12
{
	blConstantBufferDescriptorHeap::blConstantBufferDescriptorHeap(
		blDevice* device,
		UINT numDescriptors,
		std::wstring name)
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
		cbvHeapDesc.NumDescriptors = numDescriptors;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		DX12_API_CALL(device->GetDX12Device()->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(mDescriptorHeap.GetAddressOf())));
		DX12_API_CALL(mDescriptorHeap->SetName((L"[BL] " + name).c_str()));
	}
}