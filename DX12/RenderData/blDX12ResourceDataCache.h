#pragma once

#include <vector>
#include <memory>
#include <Resources/blResource.h>
#include <Resources/blResourceManager.h>
#include <optional>

namespace BoulderLeaf::Graphics::DX12
{
	struct blResourceCacheData
	{
		bool Initialized = false;
		// Add other common cache data members here
	};

	class blDX12ResourceCacheBase
	{
	public:
		blDX12ResourceCacheBase();
		virtual void UpdateCache(const blResourceId& resourceId) {};
	private:
		std::vector<blResourceCacheData> mCacheMetaData;
	public:
		bool IsCacheInitialized(const blResourceId id) const;
	protected:
		void SetCacheInitialized(const blResourceId id, bool value);
	};

	template<typename TCacheData, typename TResource>
	class blDX12ResourceDataCache: public blDX12ResourceCacheBase
	{
	public:
		constexpr static eResourceType ResourceType = TResource::ResourceType;
	private:
		std::vector<TCacheData> mCache;
	public:
		blDX12ResourceDataCache()
			:blDX12ResourceCacheBase(), mCache()
		{
			mCache.resize(blResourceManager::Get().GetResourceTypeBudget(ResourceType));
		}

		TCacheData& Get(const TResource& resource)
		{
			const blResourceId id = resource.GetId();

			TCacheData& cache = GetByIdNoInit(id);

			if (!IsCacheInitialized(id))
			{
				InitializeCache(resource, cache);
				SetCacheInitialized(id, true);
			}

			return cache;
		}

		// TODO: this function sucks. this cache interface sucks
		TCacheData& GetByIdNoInit(const blResourceId id)
		{
			TCacheData& cache = mCache[id.GetRaw()];
			return cache;
		}
	protected:
		virtual void InitializeCache(
			const TResource& resource,
			TCacheData& cache) = 0;
	};
}