#pragma once

#include <gmock/gmock.h>
#include <directx/d3dx12_core.h>

namespace BoulderLeaf::Graphics::DX12::Test
{
	class MockID3D12GraphicsCommandList : public ID3D12GraphicsCommandList
	{
	public:
        /*virtual HRESULT STDMETHODCALLTYPE Close(void) = 0;*/
        MOCK_METHOD(HRESULT, Close, (), (override));
        /*virtual HRESULT STDMETHODCALLTYPE Reset(
            _In_  ID3D12CommandAllocator* pAllocator,
            _In_opt_  ID3D12PipelineState* pInitialState) = 0;*/
        MOCK_METHOD(HRESULT, Reset, (
            _In_  ID3D12CommandAllocator* pAllocator,
            _In_opt_  ID3D12PipelineState* pInitialState), (override));
        /*virtual void STDMETHODCALLTYPE ClearState(
            _In_opt_  ID3D12PipelineState* pPipelineState) = 0;*/
        MOCK_METHOD(void, ClearState, (
            _In_opt_  ID3D12PipelineState* pPipelineState), (override));
        /*virtual void STDMETHODCALLTYPE DrawInstanced(
            _In_  UINT VertexCountPerInstance,
            _In_  UINT InstanceCount,
            _In_  UINT StartVertexLocation,
            _In_  UINT StartInstanceLocation) = 0;*/
        MOCK_METHOD(void, DrawInstanced, (
            _In_  UINT VertexCountPerInstance,
            _In_  UINT InstanceCount,
            _In_  UINT StartVertexLocation,
            _In_  UINT StartInstanceLocation), (override));
        /*virtual void STDMETHODCALLTYPE DrawIndexedInstanced(
            _In_  UINT IndexCountPerInstance,
            _In_  UINT InstanceCount,
            _In_  UINT StartIndexLocation,
            _In_  INT BaseVertexLocation,
            _In_  UINT StartInstanceLocation) = 0;*/
        MOCK_METHOD(void, DrawIndexedInstanced, (
            _In_  UINT IndexCountPerInstance,
            _In_  UINT InstanceCount,
            _In_  UINT StartIndexLocation,
            _In_  INT BaseVertexLocation,
            _In_  UINT StartInstanceLocation), (override));
        /*virtual void STDMETHODCALLTYPE Dispatch(
            _In_  UINT ThreadGroupCountX,
            _In_  UINT ThreadGroupCountY,
            _In_  UINT ThreadGroupCountZ) = 0;*/
        MOCK_METHOD(void, Dispatch, (
            _In_  UINT ThreadGroupCountX,
            _In_  UINT ThreadGroupCountY,
            _In_  UINT ThreadGroupCountZ), (override));
        /*virtual void STDMETHODCALLTYPE CopyBufferRegion(
            _In_  ID3D12Resource* pDstBuffer,
            UINT64 DstOffset,
            _In_  ID3D12Resource* pSrcBuffer,
            UINT64 SrcOffset,
            UINT64 NumBytes) = 0*/
        MOCK_METHOD(void, CopyBufferRegion, (
            _In_  ID3D12Resource* pDstBuffer,
            UINT64 DstOffset,
            _In_  ID3D12Resource* pSrcBuffer,
            UINT64 SrcOffset,
            UINT64 NumBytes), (override));
        /*virtual void STDMETHODCALLTYPE CopyTextureRegion(
            _In_  const D3D12_TEXTURE_COPY_LOCATION* pDst,
            UINT DstX,
            UINT DstY,
            UINT DstZ,
            _In_  const D3D12_TEXTURE_COPY_LOCATION* pSrc,
            _In_opt_  const D3D12_BOX* pSrcBox) = 0;*/
        MOCK_METHOD(void, CopyTextureRegion, (
            _In_  const D3D12_TEXTURE_COPY_LOCATION* pDst,
            UINT DstX,
            UINT DstY,
            UINT DstZ,
            _In_  const D3D12_TEXTURE_COPY_LOCATION* pSrc,
            _In_opt_  const D3D12_BOX* pSrcBox), (override));
       /* virtual void STDMETHODCALLTYPE CopyResource(
            _In_  ID3D12Resource* pDstResource,
            _In_  ID3D12Resource* pSrcResource) = 0;*/
        MOCK_METHOD(void, CopyResource, (
            _In_  ID3D12Resource* pDstResource,
            _In_  ID3D12Resource* pSrcResource), (override));
        /*virtual void STDMETHODCALLTYPE CopyTiles(
            _In_  ID3D12Resource* pTiledResource,
            _In_  const D3D12_TILED_RESOURCE_COORDINATE* pTileRegionStartCoordinate,
            _In_  const D3D12_TILE_REGION_SIZE* pTileRegionSize,
            _In_  ID3D12Resource* pBuffer,
            UINT64 BufferStartOffsetInBytes,
            D3D12_TILE_COPY_FLAGS Flags) = 0;*/
        MOCK_METHOD(void, CopyTiles, (
            _In_  ID3D12Resource* pTiledResource,
            _In_  const D3D12_TILED_RESOURCE_COORDINATE* pTileRegionStartCoordinate,
            _In_  const D3D12_TILE_REGION_SIZE* pTileRegionSize,
            _In_  ID3D12Resource* pBuffer,
            UINT64 BufferStartOffsetInBytes,
            D3D12_TILE_COPY_FLAGS Flags), (override));
        /*virtual void STDMETHODCALLTYPE ResolveSubresource(
            _In_  ID3D12Resource* pDstResource,
            _In_  UINT DstSubresource,
            _In_  ID3D12Resource* pSrcResource,
            _In_  UINT SrcSubresource,
            _In_  DXGI_FORMAT Format) = 0;*/
        MOCK_METHOD(void, ResolveSubresource, (
            _In_  ID3D12Resource* pDstResource,
            _In_  UINT DstSubresource,
            _In_  ID3D12Resource* pSrcResource,
            _In_  UINT SrcSubresource,
            _In_  DXGI_FORMAT Format), (override));
        /*virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(
            _In_  D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology) = 0;*/
        MOCK_METHOD(void, IASetPrimitiveTopology, (
            _In_  D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology), (override));
        /*virtual void STDMETHODCALLTYPE RSSetViewports(
            _In_range_(0, D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
            _In_reads_(NumViewports)  const D3D12_VIEWPORT* pViewports) = 0;*/
        MOCK_METHOD(void, RSSetViewports, (
            _In_range_(0, D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
            _In_reads_(NumViewports)  const D3D12_VIEWPORT* pViewports), (override));
        /*virtual void STDMETHODCALLTYPE RSSetScissorRects(
            _In_range_(0, D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects) = 0;*/
        MOCK_METHOD(void, RSSetScissorRects, (
            _In_range_(0, D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects), (override));
        /*virtual void STDMETHODCALLTYPE OMSetBlendFactor(
            _In_reads_opt_(4)  const FLOAT BlendFactor[4]) = 0;*/
        MOCK_METHOD(void, OMSetBlendFactor, (
            _In_reads_opt_(4)  const FLOAT BlendFactor[4]), (override));
        /*virtual void STDMETHODCALLTYPE OMSetStencilRef(
            _In_  UINT StencilRef) = 0;*/
        MOCK_METHOD(void, OMSetStencilRef, (
            _In_  UINT StencilRef), (override));
        /*virtual void STDMETHODCALLTYPE SetPipelineState(
            _In_  ID3D12PipelineState* pPipelineState) = 0;*/
        MOCK_METHOD(void, SetPipelineState, (
            _In_  ID3D12PipelineState* pPipelineState), (override));
        /*virtual void STDMETHODCALLTYPE ResourceBarrier(
            _In_  UINT NumBarriers,
            _In_reads_(NumBarriers)  const D3D12_RESOURCE_BARRIER* pBarriers) = 0;*/
        MOCK_METHOD(void, ResourceBarrier, (
            _In_  UINT NumBarriers,
            _In_reads_(NumBarriers)  const D3D12_RESOURCE_BARRIER* pBarriers), (override));
        /*virtual void STDMETHODCALLTYPE ExecuteBundle(
            _In_  ID3D12GraphicsCommandList* pCommandList) = 0;*/
        MOCK_METHOD(void, ExecuteBundle, (
            _In_  ID3D12GraphicsCommandList* pCommandList), (override));
        /*virtual void STDMETHODCALLTYPE SetDescriptorHeaps(
            _In_  UINT NumDescriptorHeaps,
            _In_reads_(NumDescriptorHeaps)  ID3D12DescriptorHeap* const* ppDescriptorHeaps) = 0;*/
        MOCK_METHOD(void, SetDescriptorHeaps, (
            _In_  UINT NumDescriptorHeaps,
            _In_reads_(NumDescriptorHeaps)  ID3D12DescriptorHeap* const* ppDescriptorHeaps), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRootSignature(
            _In_opt_  ID3D12RootSignature* pRootSignature) = 0;*/
        MOCK_METHOD(void, SetComputeRootSignature, (
            _In_opt_  ID3D12RootSignature* pRootSignature), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRootSignature(
            _In_opt_  ID3D12RootSignature* pRootSignature) = 0;*/
        MOCK_METHOD(void, SetGraphicsRootSignature, (
            _In_opt_  ID3D12RootSignature* pRootSignature), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRootDescriptorTable(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) = 0;*/
        MOCK_METHOD(void, SetComputeRootDescriptorTable, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRootDescriptorTable(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) = 0;*/
        MOCK_METHOD(void, SetGraphicsRootDescriptorTable, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRoot32BitConstant(
            _In_  UINT RootParameterIndex,
            _In_  UINT SrcData,
            _In_  UINT DestOffsetIn32BitValues) = 0;*/
        MOCK_METHOD(void, SetComputeRoot32BitConstant, (
            _In_  UINT RootParameterIndex,
            _In_  UINT SrcData,
            _In_  UINT DestOffsetIn32BitValues), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRoot32BitConstant(
            _In_  UINT RootParameterIndex,
            _In_  UINT SrcData,
            _In_  UINT DestOffsetIn32BitValues) = 0;*/
        MOCK_METHOD(void, SetGraphicsRoot32BitConstant, (
            _In_  UINT RootParameterIndex,
            _In_  UINT SrcData,
            _In_  UINT DestOffsetIn32BitValues), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRoot32BitConstants(
            _In_  UINT RootParameterIndex,
            _In_  UINT Num32BitValuesToSet,
            _In_reads_(Num32BitValuesToSet * sizeof(UINT))  const void* pSrcData,
            _In_  UINT DestOffsetIn32BitValues) = 0;*/
        MOCK_METHOD(void, SetComputeRoot32BitConstants, (
            _In_  UINT RootParameterIndex,
            _In_  UINT Num32BitValuesToSet,
            _In_reads_(Num32BitValuesToSet * sizeof(UINT))  const void* pSrcData,
            _In_  UINT DestOffsetIn32BitValues), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRoot32BitConstants(
            _In_  UINT RootParameterIndex,
            _In_  UINT Num32BitValuesToSet,
            _In_reads_(Num32BitValuesToSet * sizeof(UINT))  const void* pSrcData,
            _In_  UINT DestOffsetIn32BitValues) = 0;*/
        MOCK_METHOD(void, SetGraphicsRoot32BitConstants, (
            _In_  UINT RootParameterIndex,
            _In_  UINT Num32BitValuesToSet,
            _In_reads_(Num32BitValuesToSet * sizeof(UINT))  const void* pSrcData,
            _In_  UINT DestOffsetIn32BitValues), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRootConstantBufferView(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;*/
        MOCK_METHOD(void, SetComputeRootConstantBufferView, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRootConstantBufferView(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;*/
        MOCK_METHOD(void, SetGraphicsRootConstantBufferView, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRootShaderResourceView(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;*/
        MOCK_METHOD(void, SetComputeRootShaderResourceView, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRootShaderResourceView(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;*/
        MOCK_METHOD(void, SetGraphicsRootShaderResourceView, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation), (override));
        /*virtual void STDMETHODCALLTYPE SetComputeRootUnorderedAccessView(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;*/
        MOCK_METHOD(void, SetComputeRootUnorderedAccessView, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation), (override));
        /*virtual void STDMETHODCALLTYPE SetGraphicsRootUnorderedAccessView(
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;*/
        MOCK_METHOD(void, SetGraphicsRootUnorderedAccessView, (
            _In_  UINT RootParameterIndex,
            _In_  D3D12_GPU_VIRTUAL_ADDRESS BufferLocation), (override));
        /*virtual void STDMETHODCALLTYPE IASetIndexBuffer(
            _In_opt_  const D3D12_INDEX_BUFFER_VIEW* pView) = 0;*/
        MOCK_METHOD(void, IASetIndexBuffer, (
            _In_opt_  const D3D12_INDEX_BUFFER_VIEW* pView), (override));
        /*virtual void STDMETHODCALLTYPE IASetVertexBuffers(
            _In_  UINT StartSlot,
            _In_  UINT NumViews,
            _In_reads_opt_(NumViews)  const D3D12_VERTEX_BUFFER_VIEW* pViews) = 0;*/
        MOCK_METHOD(void, IASetVertexBuffers, (
            _In_  UINT StartSlot,
            _In_  UINT NumViews,
            _In_reads_opt_(NumViews)  const D3D12_VERTEX_BUFFER_VIEW* pViews), (override));
        /*virtual void STDMETHODCALLTYPE SOSetTargets(
            _In_  UINT StartSlot,
            _In_  UINT NumViews,
            _In_reads_opt_(NumViews)  const D3D12_STREAM_OUTPUT_BUFFER_VIEW* pViews) = 0*/
        MOCK_METHOD(void, SOSetTargets, (
            _In_  UINT StartSlot,
            _In_  UINT NumViews,
            _In_reads_opt_(NumViews)  const D3D12_STREAM_OUTPUT_BUFFER_VIEW* pViews), (override));
        /*virtual void STDMETHODCALLTYPE OMSetRenderTargets(
            _In_  UINT NumRenderTargetDescriptors,
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE* pRenderTargetDescriptors,
            _In_  BOOL RTsSingleHandleToDescriptorRange,
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor) = 0;*/
        MOCK_METHOD(void, OMSetRenderTargets, (
            _In_  UINT NumRenderTargetDescriptors,
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE* pRenderTargetDescriptors,
            _In_  BOOL RTsSingleHandleToDescriptorRange,
            _In_opt_  const D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor), (override));
        /*virtual void STDMETHODCALLTYPE ClearDepthStencilView(
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
            _In_  D3D12_CLEAR_FLAGS ClearFlags,
            _In_  FLOAT Depth,
            _In_  UINT8 Stencil,
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects) = 0;*/
        MOCK_METHOD(void, ClearDepthStencilView, (
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
            _In_  D3D12_CLEAR_FLAGS ClearFlags,
            _In_  FLOAT Depth,
            _In_  UINT8 Stencil,
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects), (override));
        /*virtual void STDMETHODCALLTYPE ClearRenderTargetView(
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
            _In_  const FLOAT ColorRGBA[4],
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects) = 0;*/
        MOCK_METHOD(void, ClearRenderTargetView, (
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
            _In_  const FLOAT ColorRGBA[4],
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects), (override));
        /*virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            _In_  ID3D12Resource* pResource,
            _In_  const UINT Values[4],
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects) = 0;*/
        MOCK_METHOD(void, ClearUnorderedAccessViewUint, (
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            _In_  ID3D12Resource* pResource,
            _In_  const UINT Values[4],
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects), (override));
        /*virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            _In_  ID3D12Resource* pResource,
            _In_  const FLOAT Values[4],
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects) = 0;*/
        MOCK_METHOD(void, ClearUnorderedAccessViewFloat, (
            _In_  D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
            _In_  ID3D12Resource* pResource,
            _In_  const FLOAT Values[4],
            _In_  UINT NumRects,
            _In_reads_(NumRects)  const D3D12_RECT* pRects), (override));
        /*virtual void STDMETHODCALLTYPE DiscardResource(
            _In_  ID3D12Resource* pResource,
            _In_opt_  const D3D12_DISCARD_REGION* pRegion) = 0;*/
        MOCK_METHOD(void, DiscardResource, (
            _In_  ID3D12Resource* pResource,
            _In_opt_  const D3D12_DISCARD_REGION* pRegion), (override));
        /*virtual void STDMETHODCALLTYPE BeginQuery(
            _In_  ID3D12QueryHeap* pQueryHeap,
            _In_  D3D12_QUERY_TYPE Type,
            _In_  UINT Index) = 0;*/
        MOCK_METHOD(void, BeginQuery, (
            _In_  ID3D12QueryHeap* pQueryHeap,
            _In_  D3D12_QUERY_TYPE Type,
            _In_  UINT Index), (override));
        /*virtual void STDMETHODCALLTYPE EndQuery(
            _In_  ID3D12QueryHeap* pQueryHeap,
            _In_  D3D12_QUERY_TYPE Type,
            _In_  UINT Index) = 0*/
        MOCK_METHOD(void, EndQuery, (
            _In_  ID3D12QueryHeap* pQueryHeap,
            _In_  D3D12_QUERY_TYPE Type,
            _In_  UINT Index), (override));
        /*virtual void STDMETHODCALLTYPE ResolveQueryData(
            _In_  ID3D12QueryHeap* pQueryHeap,
            _In_  D3D12_QUERY_TYPE Type,
            _In_  UINT StartIndex,
            _In_  UINT NumQueries,
            _In_  ID3D12Resource* pDestinationBuffer,
            _In_  UINT64 AlignedDestinationBufferOffset) = 0;*/
        MOCK_METHOD(void, ResolveQueryData, (
            _In_  ID3D12QueryHeap* pQueryHeap,
            _In_  D3D12_QUERY_TYPE Type,
            _In_  UINT StartIndex,
            _In_  UINT NumQueries,
            _In_  ID3D12Resource* pDestinationBuffer,
            _In_  UINT64 AlignedDestinationBufferOffset), (override));
        /*virtual void STDMETHODCALLTYPE SetPredication(
            _In_opt_  ID3D12Resource* pBuffer,
            _In_  UINT64 AlignedBufferOffset,
            _In_  D3D12_PREDICATION_OP Operation) = 0;*/
        MOCK_METHOD(void, SetPredication, (
            _In_opt_  ID3D12Resource* pBuffer,
            _In_  UINT64 AlignedBufferOffset,
            _In_  D3D12_PREDICATION_OP Operation), (override));
        /*virtual void STDMETHODCALLTYPE SetMarker(
            UINT Metadata,
            _In_reads_bytes_opt_(Size)  const void* pData,
            UINT Size) = 0;*/
        MOCK_METHOD(void, SetMarker, (
            UINT Metadata,
            _In_reads_bytes_opt_(Size)  const void* pData,
            UINT Size), (override));
        /*virtual void STDMETHODCALLTYPE BeginEvent(
            UINT Metadata,
            _In_reads_bytes_opt_(Size)  const void* pData,
            UINT Size) = 0;*/
        MOCK_METHOD(void, BeginEvent, (
            UINT Metadata,
            _In_reads_bytes_opt_(Size)  const void* pData,
            UINT Size), (override));
        /*virtual void STDMETHODCALLTYPE EndEvent(void) = 0;*/
        MOCK_METHOD(void, EndEvent, (), (override));
        /*virtual void STDMETHODCALLTYPE ExecuteIndirect(
            _In_  ID3D12CommandSignature* pCommandSignature,
            _In_  UINT MaxCommandCount,
            _In_  ID3D12Resource* pArgumentBuffer,
            _In_  UINT64 ArgumentBufferOffset,
            _In_opt_  ID3D12Resource* pCountBuffer,
            _In_  UINT64 CountBufferOffset) = 0;*/
        MOCK_METHOD(void, ExecuteIndirect, (
            _In_  ID3D12CommandSignature* pCommandSignature,
            _In_  UINT MaxCommandCount,
            _In_  ID3D12Resource* pArgumentBuffer,
            _In_  UINT64 ArgumentBufferOffset,
            _In_opt_  ID3D12Resource* pCountBuffer,
            _In_  UINT64 CountBufferOffset), (override));
        /*virtual D3D12_COMMAND_LIST_TYPE STDMETHODCALLTYPE GetType( void) = 0;*/
        MOCK_METHOD(D3D12_COMMAND_LIST_TYPE, GetType, (), (override));
        /*virtual HRESULT STDMETHODCALLTYPE GetDevice( 
            REFIID riid,
            _COM_Outptr_opt_  void **ppvDevice) = 0;*/
        MOCK_METHOD(HRESULT, GetDevice, (
            REFIID riid,
            _COM_Outptr_opt_  void** ppvDevice), (override));

        MOCK_METHOD(HRESULT, GetPrivateData, (
            _In_  REFGUID guid,
            _Inout_  UINT* pDataSize,
            _Out_writes_bytes_opt_(*pDataSize)  void* pData), (override));

        MOCK_METHOD(HRESULT, SetPrivateData, (
            _In_  REFGUID guid,
            _In_  UINT DataSize,
            _In_reads_bytes_opt_(DataSize)  const void* pData), (override));

        MOCK_METHOD(HRESULT, SetPrivateDataInterface, (
            _In_  REFGUID guid,
            _In_opt_  const IUnknown* pData), (override));

        MOCK_METHOD(HRESULT, QueryInterface, (
            REFIID riid,
            _COM_Outptr_  void** ppvObject), (override));

        MOCK_METHOD(ULONG, AddRef, (), (override));

        MOCK_METHOD(ULONG, Release, (), (override));

        MOCK_METHOD(HRESULT, SetName, (
            _In_z_  LPCWSTR Name), (override));
	};
}