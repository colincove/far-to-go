#pragma once

#include <vector>
#include <memory>
#include <Resources/blResource.h>
#include <optional>

namespace BoulderLeaf::Graphics::DX12
{
	struct blResourceCacheData
	{
		bool Initialized = false;
		// Add other common cache data members here
	};

	template<typename TCacheData, typename TResource>
	class blDX12ResourceDataCache
	{
	public:
		constexpr static eResourceType ResourceType = TResource::ResourceType;
	private:
		std::vector<TCacheData> mCache;
		std::vector<blResourceCacheData> mCacheMetaData;
	public:
		blDX12ResourceDataCache()
			:mCache(), mCacheMetaData()
		{

		}

		TCacheData& Get(const TResource& resource)
		{
			const blResourceId id = resource.GetId();

			if (mCache.size() <= id.GetRaw())
			{
				mCache.resize(id.GetRaw() + 1);
				mCacheMetaData.resize(id.GetRaw() + 1);
			}

			TCacheData& cache = mCache[id.GetRaw()];
			blResourceCacheData& metaData = mCacheMetaData[id.GetRaw()];

			if (!metaData.Initialized)
			{
				InitializeCache(resource, cache);
				metaData.Initialized = true;
			}

			return cache;
		}
	protected:
		virtual void InitializeCache(
			const TResource& resource,
			TCacheData& cache) = 0;
	};
}