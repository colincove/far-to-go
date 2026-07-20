#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <Resources/blResources.h>
#include <blDX12Buffer.h>
#include <blMesh.h>
#include <blShader.h>
#include <blCommandList.h>
#include <blDevice.h>
#include <blDX12VertexBuffer.h>
#include <blConstantBufferDescriptorHeap.h>
#include <blCompositeMesh.h>

namespace BoulderLeaf::Graphics::DX12
{
	blResourceHandleOfType<blArrayBufferResource> GetDX12ArrayBufferResource(
		blDX12ArrayBufferTranslationCache* translationCache,
		blResourceContainer* container,
		blResourceRefOfType<blArrayBufferResource> resourceRef)
	{
		blResourceHandleOfType<blArrayBufferResource> resource =
			container->CreateHandleFromRefOfType<blArrayBufferResource>(resourceRef);
		blResourceHandleOfType<blBufferDescriptionResource> desc =
			container->CreateHandleFromRefOfType<blBufferDescriptionResource>(resource->mDescriptionResourceRef);

		blResourceHandleOfType<blArrayBufferResource> dx12ArrayBuffer = resource;

		if (desc->mFormat == BufferFormat::DX12)
		{
			return resource;
		}

		const blDX12ArrayBufferTranslationCacheData& translatedCacheData =
			translationCache->GetTypedCachedData(resource);

		return translatedCacheData.translatedArrayBuffer;
	}

	blResourceHandleOfType<blArrayBufferResource> GetDX12ArrayBufferResource(
		blDX12ArrayBufferTranslationCache* translationCache,
		blResourceHandleOfType<blArrayBufferResource> resource)
	{
		return GetDX12ArrayBufferResource(
			translationCache,
			resource.GetContainer(),
			resource.GetRef()
		);
	}
}

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

		blResourceHandleOfType<blBufferDescriptionResource> desc =
			container->CreateHandleFromRefOfType<blBufferDescriptionResource>(resource->mDescriptionResourceRef);

		if (desc->mFormat == BufferFormat::DX12)
		{
			//already in the expected format. No need to translate.
			cache.translatedArrayBuffer = resource;
		}
		else
		{
			cache.translatedArrayBuffer = CreateArrayBufferResource(
				container,
				std::wstring(resource.GetName()),
				*resource,
				DX12BufferAdapter::Get());
		}
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

	void blDX12ShaderCache::InitializeCacheTemplate(const blResourceHandleOfType<blShaderResource> resource,
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
		blDX12MappedUploadBufferCache* mappedUploadBufferCache,
		blDX12ArrayBufferTranslationCache* arrayBufferTranslationCache)
		: blDX12ResourceCacheTemplate(),
		mDevice(device),
		mMappedUploadBufferCache(mappedUploadBufferCache),
		mArrayBufferTranslationCache(arrayBufferTranslationCache)
	{

	}

	void blDX12DescriptorHeapCache::InitializeCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
		blDX12DescriptorHeapCacheData& cache)
	{
		blResourceContainer* container = resource.GetContainer();
		blResourceHandleOfType<blArrayBufferResource> dx12ArrayBuffer = GetDX12ArrayBufferResource(
			mArrayBufferTranslationCache,
			resource);
		blResourceHandleOfType<blListResource> dx12Buffer = container->CreateHandleFromRefOfType<blListResource>
			(dx12ArrayBuffer->mBufferResourceRef);

		cache.descriptorHeap = std::make_unique<blConstantBufferDescriptorHeap>(
			mDevice,
			(UINT)(dx12Buffer->mCount * Constants::FrameResourceCount),
			std::wstring(resource.GetName()));

		const blDX12MappedUploadBufferCacheData& uploadBufferCache = mMappedUploadBufferCache->GetTypedCachedData(resource);

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(dx12Buffer->mElementSize));
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cache.descriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

		const size_t count = dx12Buffer->mCount;
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
	blDX12ConstantBufferDescriptorHeapCache::blDX12ConstantBufferDescriptorHeapCache(
		blDevice* device,
		blDX12MappedUploadBufferCache* mappedUploadBufferCache,
		blDX12ArrayBufferTranslationCache* arrayBufferTranslationCache)
		: blDX12ResourceCacheTemplate(),
		mDevice(device),
		mMappedUploadBufferCache(mappedUploadBufferCache),
		mArrayBufferTranslationCache(arrayBufferTranslationCache)
	{

	}

	void blDX12ConstantBufferDescriptorHeapCache::InitializeCacheTemplate(
		const blResourceHandleOfType<blConstantBufferResource> resource,
		blDX12ConstantBufferDescriptorHeapCacheData& cache)
	{
		blResourceContainer* container = resource.GetContainer();
		blResourceHandleOfType<blArrayBufferResource> instancerrayBuffer = container->CreateHandleFromRefOfType<blArrayBufferResource>(resource->mInstanceConstantBuffer);
		blResourceHandleOfType<blArrayBufferResource> dx12ArrayBuffer = GetDX12ArrayBufferResource(
			mArrayBufferTranslationCache,
			instancerrayBuffer);

		blResourceHandleOfType<blListResource> instanceBuffer = container->CreateHandleFromRefOfType<blListResource>(dx12ArrayBuffer->mBufferResourceRef);
		const uint32_t numberOfDescriptors = instanceBuffer->mCount + resource->mPassConstantBuffers.mCount;

		cache.instanceCount = instanceBuffer->mCount;
		cache.numberOfConstantBuffers = resource->mPassConstantBuffers.mCount;

		cache.descriptorHeap = std::make_unique<blConstantBufferDescriptorHeap>(
			mDevice,
			(UINT)(numberOfDescriptors * Constants::FrameResourceCount),
			std::wstring(resource.GetName()));

		const blDX12MappedUploadBufferCacheData& instanceUploadBufferCache = mMappedUploadBufferCache->GetTypedCachedData(dx12ArrayBuffer);

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(instanceBuffer->mElementSize));
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cache.descriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

		const size_t instanceCount = instanceBuffer->mCount;
		for (int frameResource = 0; frameResource < Constants::FrameResourceCount; ++frameResource)
		{
			for (const blResourceRefOfType<blArrayBufferResource>& constantArrayBufferResources : blListResource::ConstIterator<blResourceRefOfType<blArrayBufferResource>>(&resource->mPassConstantBuffers))
			{
				blResourceHandleOfType<blArrayBufferResource> dx12ArrayBufferResources = GetDX12ArrayBufferResource(
					mArrayBufferTranslationCache,
					container,
					constantArrayBufferResources);

				blResourceHandleOfType<blListResource> constantBuffer = container->CreateHandleFromRefOfType<blListResource>(dx12ArrayBufferResources->mBufferResourceRef);
				const blDX12MappedUploadBufferCacheData& uploadBufferCache = mMappedUploadBufferCache->GetTypedCachedData(dx12ArrayBufferResources);

				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = uploadBufferCache.mUploadBuffer->GetBufferLocationForIndex(frameResource, 0);
				cbvDesc.SizeInBytes = constantBuffer->mElementSize;
				mDevice->GetDX12Device()->CreateConstantBufferView(
					&cbvDesc,
					cbvHandle);
				cbvHandle.Offset(mDevice->GetCbvSrvDescriptorSize());
			}

			for (int i = 0; i < instanceCount; ++i)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = instanceUploadBufferCache.mUploadBuffer->GetBufferLocationForIndex(frameResource, i);
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
	blDX12MappedUploadBufferCache::blDX12MappedUploadBufferCache(blDevice* device, blDX12ArrayBufferTranslationCache* arrayBufferTranslationCache)
		: blDX12ResourceCacheTemplate(),
		mDevice(device),
		mArrayBufferTranslationCache(arrayBufferTranslationCache)
	{

	}

	void blDX12MappedUploadBufferCache::InitializeCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
		blDX12MappedUploadBufferCacheData& cache)
	{
		blResourceContainer* container = resource.GetContainer();
		blResourceHandleOfType<blBufferDescriptionResource> desc =
			container->CreateHandleFromRefOfType<blBufferDescriptionResource>(resource->mDescriptionResourceRef);

		blResourceHandleOfType<blArrayBufferResource> dx12ArrayBuffer = resource;

		if (desc->mFormat != BufferFormat::DX12)
		{
			const blDX12ArrayBufferTranslationCacheData& translatedCacheData =
				mArrayBufferTranslationCache->GetTypedCachedData(resource);

			dx12ArrayBuffer = translatedCacheData.translatedArrayBuffer;
		}

		blResourceHandleOfType<blListResource> dx12Buffer = container->CreateHandleFromRefOfType<blListResource>
			(dx12ArrayBuffer->mBufferResourceRef);

		cache.mUploadBuffer = std::make_unique<blDX12ListResourceUploadBuffer>(
			mDevice,
			true,
			dx12Buffer,
			std::wstring(resource.GetName())
		);

		cache.mUploadBuffer->Initialize();
		cache.mUploadBuffer->CopyAllData();
	}

	void blDX12MappedUploadBufferCache::UpdateCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
		blDX12MappedUploadBufferCacheData& cache)
	{
		blResourceContainer* container = resource.GetContainer();
		blResourceHandleOfType<blBufferDescriptionResource> desc =
			container->CreateHandleFromRefOfType<blBufferDescriptionResource>(resource->mDescriptionResourceRef);
		blResourceHandleOfType<blArrayBufferResource> dx12ArrayBuffer = resource;

		if (desc->mFormat != BufferFormat::DX12)
		{
			//by getting the cache data, we trigger an update to the data. 
			//this should ensure that the data is up-to-date for the copy below 
			//NOTE: Would a compiler remove this line due to optimization? 
			const blDX12ArrayBufferTranslationCacheData& translatedCacheData =
				mArrayBufferTranslationCache->GetTypedCachedData(resource);
		}

		//TODO: Only upload the current frame resource. Iterately update the frame resources
		cache.mUploadBuffer->CopyAllData();
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12CompositeMeshGraftingCache::blDX12CompositeMeshGraftingCache()
		: blDX12ResourceCacheTemplate()
	{

	}

	void blDX12CompositeMeshGraftingCache::InitializeCacheTemplate(const blResourceHandleOfType<blCompositeMeshResource> resource,
		blDX12CompositeMeshGraftingCacheData& cache)
	{
		cache.graftedMesh = CreateGraftedCompositeMeshResource(resource);
	}
}