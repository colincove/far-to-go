#include <ResourceCache/blDX12ResourceCacheInterface.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ResourceCacheInterface::blDX12ResourceCacheInterface()
	{

	}

	blDX12ResourceCacheGlobalInterface::blDX12ResourceCacheGlobalInterface(blGlobalRenderData* globalRenderData)
		: blDX12ResourceCacheInterface(),
		blArrayBufferTranslationCache(),
		mShaderCache(globalRenderData->device.get()),
		mMappedUploadBufferCache(globalRenderData->device.get(), &blArrayBufferTranslationCache),
		mDescriptorHeapCache(globalRenderData->device.get(), &mMappedUploadBufferCache, &blArrayBufferTranslationCache),
		mConstantBufferDescriptorHeapCache(globalRenderData->device.get(), &mMappedUploadBufferCache, &blArrayBufferTranslationCache),
		mCompositeMeshGraftingCache()
	{

	}

	const blDX12ArrayBufferTranslationCacheData& blDX12ResourceCacheGlobalInterface::GetArrayBufferTranslationCacheData(const blResourceHandleOfType<blArrayBufferResource> resource)
	{
		return blArrayBufferTranslationCache.GetTypedCachedData(resource);
	}

	const blDX12ShaderCacheData& blDX12ResourceCacheGlobalInterface::GetShaderCacheData(const blResourceHandleOfType<blShaderResource> resource)
	{
		return mShaderCache.GetTypedCachedData(resource);
	}

	const blDX12DescriptorHeapCacheData& blDX12ResourceCacheGlobalInterface::GetDescriptorHeapCacheData(const blResourceHandleOfType<blArrayBufferResource> resource)
	{
		return mDescriptorHeapCache.GetTypedCachedData(resource);
	}

	const blDX12ConstantBufferDescriptorHeapCacheData& blDX12ResourceCacheGlobalInterface::GetConstantBufferDescriptorHeapCacheData(const blResourceHandleOfType<blConstantBufferResource> resource)
	{
		return mConstantBufferDescriptorHeapCache.GetTypedCachedData(resource);
	}

	const blDX12MappedUploadBufferCacheData& blDX12ResourceCacheGlobalInterface::GetMappedUploadBufferCache(const blResourceHandleOfType<blArrayBufferResource> resource)
	{
		return mMappedUploadBufferCache.GetTypedCachedData(resource);
	}

	const blDX12CompositeMeshGraftingCacheData& blDX12ResourceCacheGlobalInterface::GetCompositeMeshGraftingCache(const blResourceHandleOfType<blCompositeMeshResource> resource)
	{
		return mCompositeMeshGraftingCache.GetTypedCachedData(resource);
	}
}