#pragma once

#include <memory>
#include <Resources/blResource.h>
#include <vector>
#include <set>
#include <map>
#include <assert.h>

namespace BoulderLeaf::Graphics
{
	class blResourceManager;

	class blResourceManager
	{
	private:
		// TODO: Implement a method of resource id recycling
		std::set<blResourceId> mReleasedResourceIds;
		// TODO: Implement a method of resource id recycling
		std::map<blResourceId, blResourceBasePtr> mResourceMap;
	public:
		blResourceId GetNewResourceId();

		void ReleaseResource(const blResourceId& id);
		bool IsValidResource(blResourceId id);
		static blResourceManager& Get();

		template<typename TResource>
		blResourcePtr<TResource> CreateResource(const TResource::DataType&& data)
		{
			blResourcePtr<TResource> resource = std::make_shared<TResource>(TResource(GetNewResourceId(), data));
			blResourceBasePtr basePtr = std::reinterpret_pointer_cast<blResourceBase>(resource);
			mResourceMap.insert({ resource->GetId(), basePtr });
			return resource;
		}

		template<typename TResource, typename... Args>
		blResourcePtr<TResource> CreateResource(Args&&... args)
		{
			blResourcePtr<TResource> resource = std::make_shared<TResource>(TResource(GetNewResourceId(), std::forward<Args>(args)...));
			blResourceBasePtr basePtr = std::reinterpret_pointer_cast<blResourceBase>(resource);
			mResourceMap.insert({ resource->GetId(), basePtr });
			return resource;
		}

		template<typename TResource>
		const blResourcePtr<TResource> GetResource(const blResourceId id) const
		{
			auto resourceMapPair = mResourceMap.find(id);
			if (resourceMapPair != mResourceMap.end())
			{
				return std::reinterpret_pointer_cast<TResource>(resourceMapPair->second);
			}

			assert(false);
			return nullptr;
		}
	};
}
