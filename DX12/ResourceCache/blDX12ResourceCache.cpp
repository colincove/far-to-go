#include <ResourceCache/blDX12ResourceCache.h>
#include <Resources/blResourcesExprimental.h>
#include <Resources/blResourceHandleOfType.h>

namespace BoulderLeaf::Graphics::DX12
{
	ResourceCacheIndex::ResourceCacheIndex(uint64_t capacity)
		: mCapacity(capacity)
	{

	}

	uint64_t ResourceCacheIndex::CalculateSize() const
	{
		return sizeof(ResourceCacheIndex) + sizeof(ResourceCacheIndexEntry) * mCapacity;
	}

	ResourceCacheIndexEntry& ResourceCacheIndex::GetEntry(const blResourceHandle& handle)
	{
		return reinterpret_cast<ResourceCacheIndexEntry&>(*(
			reinterpret_cast<byte*>(this) + 
			sizeof(ResourceCacheIndex) * handle.GetId().GetIndex()));
	}

	blDX12ResourceCache::blDX12ResourceCache()
		: mResourceGuid(blResourceGuid::New())
	{

	}

	bool blDX12ResourceCache::IsResourceLoaded(blResourceHandle resource) const
	{
		return false;
	}

	const byte& blDX12ResourceCache::GetCachedData(const blResourceHandle resource)
	{
		blResourceContainer& resourceContainer =  *resource.GetContainer();
		blResourceHandleOfType<ResourceCacheIndex> cacheIndexHandle = resourceContainer.FindByGuidOfType<ResourceCacheIndex>(mResourceGuid);

		if (!cacheIndexHandle)
		{
			cacheIndexHandle = NewContainerIndex(resourceContainer);
		}

		ResourceCacheIndexEntry& indexEntry = cacheIndexHandle->GetEntry(resource);

		if (!indexEntry.resourceHandle)
		{
			indexEntry.resourceHandle = resourceContainer.CreateResource(std::wstring(resource.GetName()), GetCacheDataSize());
			InitializeCache(resource, *indexEntry.resourceHandle);
		}

		return *indexEntry.resourceHandle;
	}

	blResourceHandleOfType<ResourceCacheIndex> blDX12ResourceCache::NewContainerIndex(blResourceContainer& resourceContainer) const
	{
		ResourceCacheIndex resourceCacheIndex(resourceContainer.Capacity());

		blResourceHandleOfType<ResourceCacheIndex> resourceHandle = blResourceHandleOfType<ResourceCacheIndex>(
			resourceContainer.CreateResourceWithGuid(L"ResourceCacheIndex", mResourceGuid, resourceCacheIndex.CalculateSize())
		);

		resourceHandle.PushData(&resourceCacheIndex);
		return resourceHandle;
	}
}