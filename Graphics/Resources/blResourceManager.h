#pragma once

#include <memory>
#include <Resources/blResource.h>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <array>

namespace BoulderLeaf::Graphics
{
	class blResourceManager;

	struct blResourceMetaData
	{
		blResourceId TypeId;
	};

	class blResourceManager
	{
	private:
		// TODO: Implement a method of resource id recycling
		std::set<blResourceId> mReleasedResourceIds;
		// TODO: Implement a method of resource id recycling
		std::map<blResourceId, blResourceBasePtr> mResourceMap;

		std::vector<blResourceMetaData> mResourceMetaData;

		bool mInitialized = false;
		unsigned int mFullResourceBudget = 0;
		std::array<unsigned int, static_cast<int>(eResourceType::Count)> mResourceTypeBudgets;
	private:
		bool ReadConfig();


		blResourceMetaData& GetResourceDataMutable(const blResourceId& id)
		{
			return mResourceMetaData[id.GetRaw()];
		}
	public:
		bool Initialize();
		blResourceId GetNewResourceId();
		blResourceId GetNewResourceIdForType(eResourceType type);

		const blResourceMetaData& GetResourceData(const blResourceId& id) const
		{
			return mResourceMetaData[id.GetRaw()];
		}

		unsigned int GetResourceTypeBudget(eResourceType type) const
		{
			return mResourceTypeBudgets[static_cast<int>(type)];
		}

		unsigned int GetFullResourceBudget() const
		{
			return mFullResourceBudget;
		}

		void ReleaseResource(const blResourceId& id);
		bool IsValidResource(blResourceId id);
		static blResourceManager& Get();

		template<typename TResource>
		blResourcePtr<TResource> CreateResource(const TResource::DataType&& data)
		{
			return CreateResourceWithName(L"", data);
		}

		template<typename TResource>
		blResourcePtr<TResource> CreateResourceWithName(const TResource::DataType&& data)
		{
			assert(mInitialized);

			const blResourceId resourceId = GetNewResourceId();

			if (!resourceId.IsValid())
			{
				return nullptr;
			}

			const blResourceId resourceTypeId = GetNewResourceIdForType(TResource::ResourceType);

			if (!resourceTypeId.IsValid())
			{
				return nullptr;
			}

			blResourceMetaData& resourceMetaData = GetResourceDataMutable(resourceId);
			resourceMetaData.TypeId = resourceTypeId;

			blResourcePtr<TResource> resource = std::make_shared<TResource>(TResource(resourceId, data));
			blResourceBasePtr basePtr = std::reinterpret_pointer_cast<blResourceBase>(resource);
			mResourceMap.insert({ resource->GetId(), basePtr });
			return resource;
		}

		template<typename TResource, typename... Args>
		blResourcePtr<TResource> CreateResource(Args&&... args)
		{
			return CreateResourceWithName<TResource>(L"Resource", std::forward<Args>(args)...);
		}

		template<typename TResource, typename... Args>
		blResourcePtr<TResource> CreateResourceWithName(std::wstring name, Args&&... args)
		{
			assert(mInitialized);

			const blResourceId resourceId = GetNewResourceId();

			if (!resourceId.IsValid())
			{
				return nullptr;
			}

			const blResourceId resourceTypeId = GetNewResourceIdForType(TResource::ResourceType);

			if (!resourceTypeId.IsValid())
			{
				return nullptr;
			}

			blResourceMetaData& resourceMetaData = GetResourceDataMutable(resourceId);
			resourceMetaData.TypeId = resourceTypeId;

			blResourcePtr<TResource> resource = std::make_shared<TResource>(TResource(resourceId, name, std::forward<Args>(args)...));
			blResourceBasePtr basePtr = std::reinterpret_pointer_cast<blResourceBase>(resource);
			mResourceMap.insert({ resource->GetId(), basePtr });
			return resource;
		}

		template<typename TResource>
		const blResourcePtr<TResource> GetResource(const blResourceId id) const
		{
			assert(mInitialized);

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
