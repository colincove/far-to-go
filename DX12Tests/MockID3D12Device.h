#pragma once

#include <gmock/gmock.h>
#include <directx/d3dx12_core.h>

namespace BoulderLeaf::Graphics::DX12::Test
{
	class MockID3D12Device : public ID3D12Device
	{
	public:

		MOCK_METHOD(HRESULT, CreateCommittedResource, (
			_In_  const D3D12_HEAP_PROPERTIES* pHeapProperties,
			D3D12_HEAP_FLAGS HeapFlags,
			_In_  const D3D12_RESOURCE_DESC* pDesc,
			D3D12_RESOURCE_STATES InitialResourceState,
			_In_opt_  const D3D12_CLEAR_VALUE* pOptimizedClearValue,
			REFIID riidResource,
			_COM_Outptr_opt_  void** ppvResource), (override));

        MOCK_METHOD(UINT, GetNodeCount, (), (override));

        MOCK_METHOD(HRESULT, CreateCommandQueue, (_In_  const D3D12_COMMAND_QUEUE_DESC* pDesc,
            REFIID riid,
            _COM_Outptr_  void** ppCommandQueue), (override));

        MOCK_METHOD(HRESULT, CreateCommandAllocator, (_In_  D3D12_COMMAND_LIST_TYPE type,
            REFIID riid,
            _COM_Outptr_  void** ppCommandAllocator), (override));

        MOCK_METHOD(HRESULT, CreateGraphicsPipelineState, (
            _In_  const D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc,
            REFIID riid,
            _COM_Outptr_  void** ppPipelineState), (override));

        MOCK_METHOD(HRESULT, CreateComputePipelineState, (
            _In_  const D3D12_COMPUTE_PIPELINE_STATE_DESC* pDesc,
            REFIID riid,
            _COM_Outptr_  void** ppPipelineState), (override));

        MOCK_METHOD(HRESULT, CreateCommandList, (
            _In_  UINT nodeMask,
            _In_  D3D12_COMMAND_LIST_TYPE type,
            _In_  ID3D12CommandAllocator* pCommandAllocator,
            _In_opt_  ID3D12PipelineState* pInitialState,
            REFIID riid,
            _COM_Outptr_  void** ppCommandList), (override));

        MOCK_METHOD(HRESULT, CheckFeatureSupport, (
            D3D12_FEATURE Feature,
            _Inout_updates_bytes_(FeatureSupportDataSize)  void* pFeatureSupportData,
            UINT FeatureSupportDataSize), (override));

        MOCK_METHOD(HRESULT, CreateDescriptorHeap, (
            _In_  const D3D12_DESCRIPTOR_HEAP_DESC* pDescriptorHeapDesc,
            REFIID riid,
            _COM_Outptr_  void** ppvHeap), (override));

        MOCK_METHOD(UINT, GetDescriptorHandleIncrementSize, (
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType), (override));

        MOCK_METHOD(HRESULT, CreateRootSignature, (
            _In_  UINT nodeMask,
            _In_reads_(blobLengthInBytes)  const void* pBlobWithRootSignature,
            _In_  SIZE_T blobLengthInBytes,
            REFIID riid,
            _COM_Outptr_  void** ppvRootSignature), (override));

        MOCK_METHOD(void, CreateConstantBufferView, (
            _In_opt_  const D3D12_CONSTANT_BUFFER_VIEW_DESC* pDesc,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor), (override));

        MOCK_METHOD(void, CreateShaderResourceView, (
            _In_opt_  ID3D12Resource* pResource,
            _In_opt_  const D3D12_SHADER_RESOURCE_VIEW_DESC* pDesc,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor), (override));

        MOCK_METHOD(void, CreateUnorderedAccessView, (
            _In_opt_  ID3D12Resource* pResource,
            _In_opt_  ID3D12Resource* pCounterResource,
            _In_opt_  const D3D12_UNORDERED_ACCESS_VIEW_DESC* pDesc,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor), (override));

        MOCK_METHOD(void, CreateRenderTargetView, (
            _In_opt_  ID3D12Resource* pResource,
            _In_opt_  const D3D12_RENDER_TARGET_VIEW_DESC* pDesc,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor), (override));

        MOCK_METHOD(void, CreateDepthStencilView, (
            _In_opt_  ID3D12Resource* pResource,
            _In_opt_  const D3D12_DEPTH_STENCIL_VIEW_DESC* pDesc,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor), (override));

        MOCK_METHOD(void, CreateSampler, (
            _In_  const D3D12_SAMPLER_DESC* pDesc,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor), (override));

        MOCK_METHOD(void, CopyDescriptors, (
            _In_  UINT NumDestDescriptorRanges,
            _In_reads_(NumDestDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE* pDestDescriptorRangeStarts,
            _In_reads_opt_(NumDestDescriptorRanges)  const UINT* pDestDescriptorRangeSizes,
            _In_  UINT NumSrcDescriptorRanges,
            _In_reads_(NumSrcDescriptorRanges)  const D3D12_CPU_DESCRIPTOR_HANDLE* pSrcDescriptorRangeStarts,
            _In_reads_opt_(NumSrcDescriptorRanges)  const UINT* pSrcDescriptorRangeSizes,
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType), (override));

        MOCK_METHOD(void, CopyDescriptorsSimple, (
            _In_  UINT NumDescriptors,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
            _In_  D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
            _In_  D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType), (override));

#if defined(_MSC_VER) || !defined(_WIN32)

        MOCK_METHOD(D3D12_RESOURCE_ALLOCATION_INFO, GetResourceAllocationInfo, (
            _In_  UINT visibleMask,
            _In_  UINT numResourceDescs,
            _In_reads_(numResourceDescs)  const D3D12_RESOURCE_DESC* pResourceDescs), (override));
#else
        MOCK_METHOD(D3D12_RESOURCE_ALLOCATION_INFO*, GetResourceAllocationInfo, (
        D3D12_RESOURCE_ALLOCATION_INFO* RetVal,
            _In_  UINT visibleMask,
            _In_  UINT numResourceDescs,
            _In_reads_(numResourceDescs)  const D3D12_RESOURCE_DESC* pResourceDescs), (override));
#endif

#if defined(_MSC_VER) || !defined(_WIN32)

        MOCK_METHOD(D3D12_HEAP_PROPERTIES, GetCustomHeapProperties, (
            _In_  UINT nodeMask,
            D3D12_HEAP_TYPE heapType), (override));
#else

        MOCK_METHOD(D3D12_HEAP_PROPERTIES*, GetCustomHeapProperties, (
            D3D12_HEAP_PROPERTIES* RetVal,
            _In_  UINT nodeMask,
            D3D12_HEAP_TYPE heapType), (override));
#endif

        MOCK_METHOD(HRESULT, CreateHeap, (
            _In_  const D3D12_HEAP_DESC* pDesc,
            REFIID riid,
            _COM_Outptr_opt_  void** ppvHeap), (override));

        MOCK_METHOD(HRESULT, CreatePlacedResource, (
            _In_  ID3D12Heap* pHeap,
            UINT64 HeapOffset,
            _In_  const D3D12_RESOURCE_DESC* pDesc,
            D3D12_RESOURCE_STATES InitialState,
            _In_opt_  const D3D12_CLEAR_VALUE* pOptimizedClearValue,
            REFIID riid,
            _COM_Outptr_opt_  void** ppvResource), (override));

        MOCK_METHOD(HRESULT, CreateReservedResource, (
            _In_  const D3D12_RESOURCE_DESC* pDesc,
            D3D12_RESOURCE_STATES InitialState,
            _In_opt_  const D3D12_CLEAR_VALUE* pOptimizedClearValue,
            REFIID riid,
            _COM_Outptr_opt_  void** ppvResource), (override));

        MOCK_METHOD(HRESULT, CreateSharedHandle, (
            _In_  ID3D12DeviceChild* pObject,
            _In_opt_  const SECURITY_ATTRIBUTES* pAttributes,
            DWORD Access,
            _In_opt_  LPCWSTR Name,
            _Out_  HANDLE* pHandle), (override));

        MOCK_METHOD(HRESULT, OpenSharedHandle, (
            _In_  HANDLE NTHandle,
            REFIID riid,
            _COM_Outptr_opt_  void** ppvObj), (override));

        MOCK_METHOD(HRESULT, OpenSharedHandleByName, (
            _In_  LPCWSTR Name,
            DWORD Access, 
            /* [annotation][out] */
            _Out_  HANDLE* pNTHandle), (override));

        MOCK_METHOD(HRESULT, MakeResident, (
            UINT NumObjects,
            _In_reads_(NumObjects)  ID3D12Pageable* const* ppObjects), (override));

        MOCK_METHOD(HRESULT, Evict, (
            UINT NumObjects,
            _In_reads_(NumObjects)  ID3D12Pageable* const* ppObjects), (override));

        MOCK_METHOD(HRESULT, CreateFence, (
            UINT64 InitialValue,
            D3D12_FENCE_FLAGS Flags,
            REFIID riid,
            _COM_Outptr_  void** ppFence), (override));

        MOCK_METHOD(HRESULT, GetDeviceRemovedReason, (), (override));

        MOCK_METHOD(void, GetCopyableFootprints, (
            _In_  const D3D12_RESOURCE_DESC* pResourceDesc,
            _In_range_(0, D3D12_REQ_SUBRESOURCES)  UINT FirstSubresource,
            _In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource)  UINT NumSubresources,
            UINT64 BaseOffset,
            _Out_writes_opt_(NumSubresources)  D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
            _Out_writes_opt_(NumSubresources)  UINT* pNumRows,
            _Out_writes_opt_(NumSubresources)  UINT64* pRowSizeInBytes,
            _Out_opt_  UINT64* pTotalBytes), (override));

        MOCK_METHOD(HRESULT, CreateQueryHeap, (
            _In_  const D3D12_QUERY_HEAP_DESC* pDesc,
            REFIID riid,
            _COM_Outptr_opt_  void** ppvHeap), (override));

        MOCK_METHOD(HRESULT, SetStablePowerState, (
            BOOL Enable), (override));

        MOCK_METHOD(HRESULT, CreateCommandSignature, (
            _In_  const D3D12_COMMAND_SIGNATURE_DESC* pDesc,
            _In_opt_  ID3D12RootSignature* pRootSignature,
            REFIID riid,
            _COM_Outptr_opt_  void** ppvCommandSignature), (override));

        MOCK_METHOD(void, GetResourceTiling, (
            _In_  ID3D12Resource* pTiledResource,
            _Out_opt_  UINT* pNumTilesForEntireResource,
            _Out_opt_  D3D12_PACKED_MIP_INFO* pPackedMipDesc,
            _Out_opt_  D3D12_TILE_SHAPE* pStandardTileShapeForNonPackedMips,
            _Inout_opt_  UINT* pNumSubresourceTilings,
            _In_  UINT FirstSubresourceTilingToGet,
            _Out_writes_(*pNumSubresourceTilings) D3D12_SUBRESOURCE_TILING* pSubresourceTilingsForNonPackedMips), (override));

#if defined(_MSC_VER) || !defined(_WIN32)
        MOCK_METHOD(LUID, GetAdapterLuid, (), (override));
#else
        virtual LUID* STDMETHODCALLTYPE GetAdapterLuid(
            LUID* RetVal) = 0;
        MOCK_METHOD(LUID*, GetAdapterLuid, (LUID* RetVal), (override));
#endif

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