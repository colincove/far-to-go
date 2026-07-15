#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <Resources/blResourcesExprimental.h>
#include <blDX12Buffer.h>
#include <blMesh.h>
#include <blShader.h>
#include <blCommandList.h>
#include <blDevice.h>
#include <blDX12VertexBuffer.h>
#include <blConstantBufferDescriptorHeap.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ArrayBufferTranslationCache::blDX12ArrayBufferTranslationCache()
		: blDX12ResourceCacheTemplate()
	{

	}

	void blDX12ArrayBufferTranslationCache::InitializeCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
		blDX12ArrayBufferTranslationCacheData& cache)
	{
		blResourceContainer* container = resource.GetContainer();
		cache.translatedArrayBuffer = CreateArrayBufferResource(
			container,
			std::wstring(resource.GetName()),
			*resource,
			DX12BufferAdapter::Get());
	}

	void blDX12ArrayBufferTranslationCache::UpdateCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
		blDX12ArrayBufferTranslationCacheData& cache)
	{
		blResourceContainer* resourceContainer = resource.GetContainer();

		blResourceHandleOfType<blListResource> sourceListResourceHandle = blResourceHandleOfType<blListResource>(
			resourceContainer->CreateHandleFromRefOfType<blListResource>(resource->mBufferResourceRef));

		blResourceHandleOfType<blBufferDescriptionResource> DescriptionResourceHandle = blResourceHandleOfType<blBufferDescriptionResource>(
			resourceContainer->CreateHandleFromRefOfType<blBufferDescriptionResource>(resource->mDescriptionResourceRef));

		blResourceHandleOfType<blListResource> arrayData = resourceContainer->CreateHandleFromRefOfType<blListResource>(cache.translatedArrayBuffer->mBufferResourceRef);

		BufferDescription description = DescriptionResourceHandle->GetBufferDescription();

		MarshalBuffer(
			sourceListResourceHandle->mCount,
			description.elementDescriptions,
			[&sourceListResourceHandle](size_t index) -> const byte*
			{
				return &sourceListResourceHandle->Get<byte>((uint32_t)index);
			},
			[&arrayData](size_t index) -> byte*
			{
				return &arrayData->GetMutable<byte>((uint32_t)index);
			},
			DX12BufferAdapter::Get()
		);

		cache.translatedArrayBuffer.MarkDirty();
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12UploadBufferCache::blDX12UploadBufferCache(blDevice* device, blCommandList* commandList)
		: blDX12ResourceCacheTemplate(), mDevice(device), mCommandList(commandList)
	{

	}

	void blDX12UploadBufferCache::InitializeCacheTemplate(const blResourceHandleOfType<blListResource> resource,
		blDX12UploadBufferCacheData& cache)
	{
		using namespace DirectX;

		cache.mUploadBuffer = std::make_unique<blDX12UploadBuffer>();

		const blListResource& resourceData = *resource;

		GenerateSingleSubresources(
			mDevice->GetDX12Device().Get(),
			mCommandList->GetCommandListPtr().Get(),
			resourceData.mCount * resourceData.mElementSize,
			resourceData.mElementSize,
			resourceData.mCount,
			&resourceData.Get<byte>(0),
			*cache.mUploadBuffer.get()
			);

		cache.mUploadBuffer->SetResourceNames(std::wstring(resource.GetName()));
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ShaderCache::blDX12ShaderCache(blDevice* device)
		: blDX12ResourceCacheTemplate(),
		mDevice(device)
	{

	}

	void blDX12ShaderCache::InitializeCacheTemplate(const blResourceHandleOfType<blShaderResource_exp> resource,
		blDX12ShaderCacheData& cache)
	{
		LoadShaderDataFromResource_exp(
			resource,
			mDevice,
			cache.shaderData
		);

		cache.shaderPSO = std::make_unique<blPSO>(
			mDevice->GetDX12Device().Get(),
			cache.shaderData.RootSignature->GetRootSignature().Get(),
			cache.shaderData.InputLayout,
			cache.shaderData.mvsByteCode,
			cache.shaderData.mpsByteCode,
			std::wstring(resource.GetName()));
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12DescriptorHeapCache::blDX12DescriptorHeapCache(blDevice* device,
		blDX12MappedUploadBufferCache* mappedUploadBufferCache)
		: blDX12ResourceCacheTemplate(),
		mDevice(device),
		mMappedUploadBufferCache(mappedUploadBufferCache)
	{

	}

	void blDX12DescriptorHeapCache::InitializeCacheTemplate(const blResourceHandleOfType<blListResource> resource,
		blDX12DescriptorHeapCacheData& cache)
	{
		cache.descriptorHeap = std::make_unique<blConstantBufferDescriptorHeap>(
			mDevice,
			(UINT)(resource->mCount * Constants::FrameResourceCount),
			std::wstring(resource.GetName()));

		const blDX12MappedUploadBufferCacheData& uploadBufferCache = mMappedUploadBufferCache->GetTypedCachedData(resource);

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(resource->mElementSize));
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cache.descriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

		const size_t count = resource->mCount;
		for (int frameResource = 0; frameResource < Constants::FrameResourceCount; ++frameResource)
		{
			for (int i = 0; i < count; ++i)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = uploadBufferCache.mUploadBuffer->GetBufferLocationForIndex(frameResource, i);
				cbvDesc.SizeInBytes = objCBByteSize;
				mDevice->GetDX12Device()->CreateConstantBufferView(
					&cbvDesc,
					cbvHandle);
				cbvHandle.Offset(mDevice->GetCbvSrvDescriptorSize());
			}
		}
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12MappedUploadBufferCache::blDX12MappedUploadBufferCache(blDevice* device)
		: blDX12ResourceCacheTemplate(),
		mDevice(device)
	{

	}

	void blDX12MappedUploadBufferCache::InitializeCacheTemplate(const blResourceHandleOfType<blListResource> resource,
		blDX12MappedUploadBufferCacheData& cache)
	{
		cache.mUploadBuffer = std::make_unique<blDX12ListResourceUploadBuffer>(
			mDevice,
			true,
			resource,
			std::wstring(resource.GetName())
		);

		cache.mUploadBuffer->Initialize();
		cache.mUploadBuffer->CopyAllData();
	}

	void blDX12MappedUploadBufferCache::UpdateCacheTemplate(const blResourceHandleOfType<blListResource> resource,
		blDX12MappedUploadBufferCacheData& cache)
	{
		//TODO: Only upload the current frame resource. Iterately update the frame resources
		cache.mUploadBuffer->CopyAllData();
	}
}