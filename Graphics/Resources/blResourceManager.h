#pragma once

#include <memory>
#include <Resources/blResource.h>
#include <vector>
#include <set>

namespace BoulderLeaf::Graphics
{
	class blResourceManager;

	class blResourceManager
	{
	private:
		// TODO: Implement a method of resource id recycling
		std::set<blResourceId> mReleasedResourceIds;
	public:
		blResourceId GetNewResourceId();

		void ReleaseResource(const blResourceId& id);
		bool IsValidResource(blResourceId id);
		static blResourceManager& Get();

		template<typename TResource>
		blResourcePtr<TResource> CreateResource(const TResource::DataType&& data)
		{
			blResourcePtr<TResource> resource = std::make_shared<TResource>(TResource(GetNewResourceId(), data));
			return resource;
		}

		template<typename TResource, typename... Args>
		blResourcePtr<TResource> CreateResource(Args&&... args)
		{
			blResourcePtr<TResource> resource = std::make_shared<TResource>(TResource(GetNewResourceId(), std::forward<Args>(args)...));
			return resource;
		}
	};
}
