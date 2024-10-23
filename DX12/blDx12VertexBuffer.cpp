#include <directx/d3dx12_core.h>
#include <directx/D3dx12.h>
#include "blDx12VertexBuffer.h"
#include <directx/d3d12.h>
#include<cassert>

namespace BoulderLeaf::Graphics::DX12
{
	CD3DX12_RESOURCE_DESC Buffer(
		UINT64 width,
		D3D12_RESOURCE_FLAGS flags,
		UINT64 alignment)
	{
		return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER,
			alignment, width, 1, 1, 1,
			DXGI_FORMAT_UNKNOWN, 1, 0,
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}

	ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		const void* initData,
		UINT64 byteSize,
		ComPtr<ID3D12Resource>& uploadBuffer
	)
	{
		return CreateDefaultBuffer(
			device,
			commandList,
			initData,
			byteSize,
			uploadBuffer,
			[](UPDATE_SUBRESOURCES_PARAMETERS) 
			{
				return UpdateSubresources<1>(
					UPDATE_SUBRESOURCES_PARAMETERS_PASSTHROUGH
				);
			}
		);
	}

	ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		const void* initData,
		UINT64 byteSize,
		ComPtr<ID3D12Resource>& uploadBuffer,
		UpdateSubresourcesFunction updateSubresourcesFunction
	)
	{
		ComPtr<ID3D12Resource> defaultBuffer;
		const CD3DX12_HEAP_PROPERTIES heapPropertiesDefault(D3D12_HEAP_TYPE_DEFAULT);
		const CD3DX12_RESOURCE_DESC bufferDesc(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
		const HRESULT bufferResult = device->CreateCommittedResource(
			&heapPropertiesDefault,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(defaultBuffer.GetAddressOf())
		);

		assert(bufferResult == S_OK);

		const CD3DX12_HEAP_PROPERTIES heapPropertiesUpload(D3D12_HEAP_TYPE_UPLOAD);
		const HRESULT uploadResourceResult = device->CreateCommittedResource(
			&heapPropertiesUpload,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())
		);

		assert(uploadResourceResult == S_OK);

		const D3D12_SUBRESOURCE_DATA subresoureData(initData, byteSize, byteSize);

		const CD3DX12_RESOURCE_BARRIER resourceBarrierStateOne(CD3DX12_RESOURCE_BARRIER::Transition(
			defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		));

		const CD3DX12_RESOURCE_BARRIER resourceBarrierStateTwo(CD3DX12_RESOURCE_BARRIER::Transition(
			defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ
		));

		commandList->ResourceBarrier(
			1,
			&resourceBarrierStateOne
		);

		updateSubresourcesFunction(
			commandList,
			defaultBuffer.Get(),
			uploadBuffer.Get(),
			0,
			0,
			1,
			&subresoureData
		);

		commandList->ResourceBarrier(
			1,
			&resourceBarrierStateTwo
		);

		//note: must keep this return value alive until the command list has been executed
		return defaultBuffer;
	}
}