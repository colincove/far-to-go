#pragma once

#include <vector>
#include <Resources/blResourceGuid.h>
#include <Resources/blResourceHandle.h>

namespace BoulderLeaf
{
	class blResourceContainer;

	template<typename T>
	class blResourceHandleOfType;

	class blResourceHandle;

	namespace Graphics
	{
		struct blInlineMesh;
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	struct ResourceCacheIndexEntry
	{
		blResourceHandle resourceHandle;
	};

	class ResourceCacheIndex
	{
	private:
		uint64_t mCapacity;
	public:
		ResourceCacheIndex(uint64_t capacity);
		uint64_t CalculateSize() const;
		ResourceCacheIndexEntry& GetEntry(const blResourceHandle& handle);
	};

	class blDX12ResourceCache
	{
	public:

		blDX12ResourceCache();

		bool IsResourceLoaded(blResourceHandle resource) const;

	protected:
		const byte& GetCachedData(const blResourceHandle resource);
		virtual uint64_t GetCacheDataSize() const = 0;
		virtual void InitializeCache(const blResourceHandle resource,
			byte& cache) = 0;

	private:
		blResourceGuid mResourceGuid;

		blResourceHandleOfType<ResourceCacheIndex> NewContainerIndex(blResourceContainer& resourceContainer) const;
	};
}