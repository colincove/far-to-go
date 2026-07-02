#pragma once

#include <ResourceCache/blDX12ResourceCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	template<typename TResource, typename TCache>
	class blDX12ResourceCacheTemplate : public blDX12ResourceCache
	{
	public:
		const TCache& GetTypedCachedData(const blResourceHandleOfType<TResource> resource) 
		{
			const byte& cacheData = GetCachedData(resource);
			return reinterpret_cast<const TCache&>(cacheData);
		}
	protected:
		virtual uint64_t GetCacheDataSize() const override final
		{
			return sizeof(TCache);
		}

		virtual void InitializeCache(const blResourceHandle resource,
			byte& cache) override final
		{
			InitializeCacheTemplate(
				blResourceHandleOfType<TResource>(resource),
				reinterpret_cast<TCache&>(cache)
			);
		}

		virtual void InitializeCacheTemplate(const blResourceHandleOfType<TResource> resource,
			TCache& cache) = 0;
	};
}