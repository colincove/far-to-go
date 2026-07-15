#pragma once

#include <blDX12Core.h>
#include <functional>
#include <blMesh.h>
#include <blDX12MeshGeometry.h>

struct CD3DX12_RESOURCE_DESC;

namespace BoulderLeaf::Graphics::DX12
{
	#define UPDATE_SUBRESOURCES_PARAMETERS											\
		_In_ ID3D12GraphicsCommandList* pCmdList,									\
		_In_ ID3D12Resource* pDestinationResource,									\
		_In_ ID3D12Resource* pIntermediate,											\
		UINT64 IntermediateOffset,													\
		_In_range_(0, MaxSubresources) UINT FirstSubresource,						\
		_In_range_(1, MaxSubresources - FirstSubresource) UINT NumSubresources,		\
		_In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData			\

	#define UPDATE_SUBRESOURCES_PARAMETERS_TYPES									\
		_In_ ID3D12GraphicsCommandList*,											\
		_In_ ID3D12Resource*,														\
		_In_ ID3D12Resource*,														\
		UINT64,																		\
		_In_range_(0, MaxSubresources) UINT,										\
		_In_range_(1, MaxSubresources - FirstSubresource) UINT,						\
		_In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA*					\

	#define UPDATE_SUBRESOURCES_PARAMETERS_PASSTHROUGH								\
		pCmdList,																	\
		pDestinationResource,														\
		pIntermediate,																\
		IntermediateOffset,															\
		FirstSubresource,															\
		NumSubresources,															\
		pSrcData																	\

	using UpdateSubresourcesFunction = std::function<UINT64(UPDATE_SUBRESOURCES_PARAMETERS)>;

	CD3DX12_RESOURCE_DESC Buffer(
		UINT64 width,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		UINT64 alignment = 0);

	ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		const void* initData,
		UINT64 byteSize,
		ComPtr<ID3D12Resource>& uploadBuffer
	);

	ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		const void* initData,
		UINT64 byteSize,
		ComPtr<ID3D12Resource>& uploadBuffer,
		UpdateSubresourcesFunction updateSubresourcesFunction
	);

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
		SubmeshGeometry& submesh);

	void GenerateSingleSubresources(
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		uint64_t bufferWidth,
		uint64_t elementWidth,
		uint32_t count,
		const byte* bufferData,
		blDX12UploadBuffer& uploadBuffer);

	void GenerateSubresources(
		const blMeshStorage& storage,
		ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList,
		blDX12Mesh& meshGeometry,
		SubmeshGeometry& submesh);
}