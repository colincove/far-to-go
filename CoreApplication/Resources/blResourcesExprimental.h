#pragma once

#include <memory>
#include <cstdint>
#include <blTypes.h>
#include <string>
#include <vector>
#include <blCore.h>
#include <Resources/blResourceGuid.h>
#include <Resources/blResourceHandle.h>

namespace BoulderLeaf
{
	class blResourceContainerPool;
	class blResourceHandle;

	class blResourceContainer
	{
		friend blResourceContainerPool;
	public:
		struct Entry
		{
			blResourceGuid mGuid;
			uint64_t mSize;
			uint64_t mOffset;
			uint64_t mNameLength;

			bool IsAvailableForAllocation() const;
			void ReleaseResource();
			bool IsMemoryFragment() const;
		};

	private:
		std::unique_ptr<Entry[]> mRegistry;
		std::unique_ptr<byte[]> mHeap;
		uint64_t mHeapEnd;
		resource_container_id mId;
		blResourceContainerPool* owningPool;
		uint32_t mNumberOfEntries;

		blResourceContainer(
			resource_container_id id, 
			uint64_t heapSize, 
			uint32_t numberOfEntries,
			blResourceContainerPool* owningPool);

	public:

		void Defrag();
		void Resize(uint64_t heapSize);
		void Release();
		void Release(blResourceId id);

		blResourceHandle CreateHandleFromRef(blResourceRef ref) const
		{
			return blResourceHandle(ref.GetId(), const_cast<blResourceContainer*>(this));
		}

		blResourceHandle CreateResource(const std::wstring& name, uint64_t size);
		blResourceHandle CreateResourceWithGuid(const std::wstring& name, blResourceGuid guid, uint64_t size);

		template<typename TResource, typename... Args>
		blResourceHandleOfType<TResource> CreateResourceOfType(const std::wstring name, Args&&... args)
		{
			blResourceHandle handle = CreateResource(name, sizeof(TResource));
			blResourceHandleOfType<TResource> handleOfType(handle);
			*handleOfType = TResource(std::forward<Args>(args)...);
			return handleOfType;
		}

		// Overload that allows caller to specify explicit allocation size as uint64_t
		template<typename TResource>
		blResourceHandleOfType<TResource> CreateResourceOfType(const std::wstring name, uint64_t explicitSize)
		{
			blResourceHandle handle = CreateResource(name, explicitSize);
			return blResourceHandleOfType<TResource>(handle);
		}

		template<typename TResource, typename... Args>
		blResourceHandleOfType<TResource> CreateResourceWithGuidOfType(const std::wstring name, blResourceGuid guid, Args&&... args)
		{
			blResourceHandle handle = CreateResourceWithGuid(name, guid, sizeof(TResource));
			return blResourceHandleOfType<TResource>(handle);
		}

		const byte* GetData(blResourceId id) const;
		byte* GetDataMutable(blResourceId id);
		const blResourceGuid& GetDataGuid(blResourceId id) const;
		const std::wstring_view GetDataName(blResourceId id) const;
		uint32_t Capacity() const;
		blResourceHandle FindByGuid(blResourceGuid& guid);

		template<typename TResource>
		blResourceHandleOfType<TResource> FindByGuidOfType(blResourceGuid& guid)
		{
			if (blResourceHandle foundHandle = FindByGuid(guid))
			{
				return blResourceHandleOfType<TResource>(foundHandle);
			}

			return blResourceHandleOfType<TResource>();
		}
	};

	class blResourceContainerPool
	{
	private:
		std::vector<blResourceContainer*> mContainerPool;
		resource_container_id mCurrentId;
	public:
		blResourceContainerPool();
		blResourceContainer* GetContainer(
			uint64_t heapSize,
			uint32_t numberOfEntries);

		void Return(const blResourceContainer& container);
	};
}
