#include <directx/d3dx12_core.h>
#include <directx/D3dx12.h>
#include <blDx12VertexBuffer.h>
#include <directx/d3d12.h>
#include <cassert>

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
		DX12_API_CALL(device->CreateCommittedResource(
			&heapPropertiesDefault,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(defaultBuffer.GetAddressOf())
		));

		const CD3DX12_HEAP_PROPERTIES heapPropertiesUpload(D3D12_HEAP_TYPE_UPLOAD);
		DX12_API_CALL(device->CreateCommittedResource(
			&heapPropertiesUpload,
			D3D12_HEAP_FLAG_NONE,
			&bufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(uploadBuffer.GetAddressOf())
		));

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

	void GenerateSingleSubresources(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		uint64_t bufferWidth,
		uint64_t elementWidth,
		uint32_t count,
		const byte* bufferData,
		blDX12UploadBuffer& uploadBuffer)
	{
		DX12_API_CALL(D3DCreateBlob(bufferWidth, &uploadBuffer.bufferCPU));
		CopyMemory(uploadBuffer.bufferCPU->GetBufferPointer(), bufferData, bufferWidth);

		uploadBuffer.bufferGPU = CreateDefaultBuffer(
			device,
			commandList,
			bufferData,
			bufferWidth,
			uploadBuffer.bufferUploader);

		uploadBuffer.byteStride = elementWidth;
		uploadBuffer.bufferByteSize = bufferWidth;
	}

	void GenerateSubresources(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		uint64_t vertexBufferWidth,
		uint64_t indexBufferWidth,
		uint64_t vertexWidth,
		uint32_t indexCount,
		const byte* vertexBufferData,
		const uint16_t* indexBufferData,
		blDX12Mesh& meshGeometry,
		SubmeshGeometry& submesh)
	{
		DX12_API_CALL(D3DCreateBlob(vertexBufferWidth, &meshGeometry.VertexBufferCPU));
		CopyMemory(meshGeometry.VertexBufferCPU->GetBufferPointer(), vertexBufferData, vertexBufferWidth);
		DX12_API_CALL(D3DCreateBlob(indexBufferWidth, &meshGeometry.IndexBufferCPU));
		CopyMemory(meshGeometry.IndexBufferCPU->GetBufferPointer(), indexBufferData, indexBufferWidth);

		meshGeometry.VertexBufferGPU = CreateDefaultBuffer(
			device,
			commandList,
			vertexBufferData,
			vertexBufferWidth,
			meshGeometry.VertexBufferUploader);

		meshGeometry.IndexBufferGPU = CreateDefaultBuffer(
			device,
			commandList,
			indexBufferData,
			indexBufferWidth,
			meshGeometry.IndexBufferUploader);

		meshGeometry.VertexByteStride = vertexWidth;
		meshGeometry.VertexBufferByteSize = vertexBufferWidth;
		meshGeometry.IndexFormat = DXGI_FORMAT_R16_UINT;
		meshGeometry.IndexBufferByteSize = indexBufferWidth;

		submesh = SubmeshGeometry();
		submesh.IndexCount = indexCount;
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;
	}
}