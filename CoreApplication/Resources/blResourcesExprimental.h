#pragma once

#include <memory>
#include <cstdint>
#include <blTypes.h>
#include <string>
#include <vector>
#include <blCore.h>
#include <Resources/blResourceGuid.h>
#include <Resources/blResourceHandle.h>
#include <Resources/blResourceHandleOfType.h>

namespace BoulderLeaf
{
	class blResourceContainerPool;
	class blResourceHandle;
	class blResourceContainer;

	class blResourceStream
	{
		struct WriteResult
		{
			byte* mStart;
			byte* mEnd;
			byte* mResource;

			uint64_t DataWritten()
			{
				return mEnd - mStart;
			}
		};

		byte* mHead;
		byte* mResource;
		byte* mStart;

	public:
		blResourceStream(byte* resource, uint64_t resourceOffset)
			: mResource(resource), 
			mStart(resource + resourceOffset), 
			mHead(resource + resourceOffset)
		{

		}

		template<typename T>
		uint64_t GetMemberToDataOffset(const T* memberPointer) const
		{
			return mHead - reinterpret_cast<const byte*>(memberPointer);
		}

		template<typename T>
		WriteResult Write(const T& data)
		{
			*mHead = reinterpret_cast<const byte&>(data);
			AllocateExplicit(sizeof(T));
			return { mHead - sizeof(T) , mHead, mResource };
		}

		template<typename T>
		WriteResult Copy(const T* start, uint64_t length)
		{
			memcpy(mHead, start, length);
			AllocateExplicit(length);
			return { mHead - length , mHead, mResource };
		}

		template<typename TResource, typename... Args>
		WriteResult WriteResource(Args&&... args)
		{
			byte* allocationPtr = mHead;
			AllocateExplicit(sizeof(TResource));
			TResource& resource = *new (allocationPtr) TResource(*this, std::forward<Args>(args)...);
			return { mHead - sizeof(TResource) , mHead, mResource };
		}

		void AllocateExplicit(uint64_t size)
		{
			mHead += size;
		}

		uint64_t GetCurrentOffset() const
		{
			return mHead - mStart;
		}
	};

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
		uint64_t mHeapSize;
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

		template<typename TResource>
		blResourceHandleOfType<TResource> CreateHandleFromRefOfType(blResourceRefOfType<TResource> ref) const
		{
			return blResourceHandleOfType<TResource>(
				blResourceHandle(ref.GetId(), const_cast<blResourceContainer*>(this)));
		}

		blResourceHandle CreateResource(const std::wstring& name, uint64_t size);
		blResourceHandle CreateResourceWithGuid(const std::wstring& name, blResourceGuid guid, uint64_t size);

		template<typename TResource, typename... Args>
		blResourceHandleOfType<TResource> CreateResourceOfTypeWithDynamicSize(const std::wstring name, Args&&... args)
		{
			byte* startOfResourceBuffer = mHeap.get() + mHeapEnd;
			blResourceStream dynamicResourceStream(startOfResourceBuffer, sizeof(TResource));
			new (startOfResourceBuffer) TResource(dynamicResourceStream, std::forward<Args>(args)...);
			
			const uint64_t totalSize = sizeof(TResource) + dynamicResourceStream.GetCurrentOffset();
			blResourceHandle handle = CreateResource(name, totalSize);
			blResourceHandleOfType<TResource> handleOfType(handle);
			return handleOfType;
		}

		template<typename TResource, typename... Args>
		blResourceHandleOfType<TResource> CreateResourceOfType(const std::wstring name, Args&&... args)
		{
			blResourceHandle handle = CreateResource(name, sizeof(TResource));
			blResourceHandleOfType<TResource> handleOfType(handle);
			new (&handleOfType) TResource(std::forward<Args>(args)...);
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
		std::vector<blResourceContainer*> mOwnedContainers;
		resource_container_id mCurrentId;
	public:
		blResourceContainerPool();
		~blResourceContainerPool();
		blResourceContainerPool(blResourceContainerPool&& other) = delete;
		blResourceContainerPool(const blResourceContainerPool& other) = delete;
		blResourceContainerPool& operator=(blResourceContainerPool&& other) noexcept = delete;

		void PrecacheContainer(
			uint64_t heapSize,
			uint32_t numberOfEntries);

		blResourceContainer* GetContainer(
			uint64_t heapSize,
			uint32_t numberOfEntries);

		void Return(blResourceContainer& container);
	};



	struct blBaseResource
	{
		uint64_t mLocalStreamOffset;
		uint64_t mMemberToDataOffset;
		blBaseResource(blResourceStream& stream);
		blBaseResource();
		blBaseResource(uint64_t mLocalStreamOffset);
		blBaseResource(const blBaseResource& other) = delete;
		blBaseResource(blBaseResource&& other) = delete;
	};

	struct blListResource : public blBaseResource
	{
		uint32_t mCount;
		uint64_t mElementSize;
		uint64_t mDataOffset;

		blListResource(const blListResource&& other);

		blListResource(blResourceStream& stream,
			uint32_t count,
			uint64_t elementSize,
			const void* data);

		blListResource(blResourceStream& stream,
			uint32_t count,
			uint64_t elementSize);

		template<typename T>
		static blListResource New(blResourceStream& stream, uint32_t count)
		{
			return blListResource(stream, count, sizeof(T));
		}

		template<typename T>
		const T& Get(uint32_t index) const
		{
			return *reinterpret_cast<const T*>(reinterpret_cast<const byte*>(this) + mDataOffset + (index * mElementSize));
		}

		template<typename T>
		T& GetMutable(uint32_t index)
		{
			return *reinterpret_cast<T*>(reinterpret_cast<byte*>(this) + mDataOffset  + (index * mElementSize));
		}

		template<typename T>
		T& operator[](uint32_t index)
		{
			return GetMutable<T>(index);
		}

		template<typename T>
		const T& operator[](uint32_t index) const
		{
			return Get<T>(index);
		}

		template<typename T>
		const T* Data() const
		{
			return reinterpret_cast<const T*>(reinterpret_cast<const byte*>(this) + mDataOffset);
		}

		template<typename T>
		T* Data()
		{
			return reinterpret_cast<T*>(reinterpret_cast<byte*>(this) + mDataOffset);
		}

		class Iterator
		{
		private:
			blListResource* mList;
			uint32_t mCurrentIndex;
		public:
			Iterator(blListResource* list, uint32_t currentIndex)
				: mList(list), mCurrentIndex(currentIndex)
			{

			}

			Iterator(blListResource* list)
				: Iterator(list, mCurrentIndex)
			{

			}

			byte& operator*() const
			{
				return mList->GetMutable<byte>(mCurrentIndex);
			}

			Iterator& operator++()
			{
				++mCurrentIndex;
				return *this;
			}

			bool operator!=(const Iterator& other) const
			{
				return mCurrentIndex != other.mCurrentIndex;
			}

			Iterator begin() const
			{
				return Iterator(mList, 0);
			}

			Iterator end() const
			{
				return Iterator(mList, mList->mCount);
			}
		};

		class ConstIterator
		{
		private:
			const blListResource* mList;
			uint32_t mCurrentIndex;
		public:
			ConstIterator(const blListResource* list, uint32_t currentIndex)
				: mList(list), mCurrentIndex(currentIndex)
			{

			}

			ConstIterator(blListResource* list)
				: ConstIterator(list, mCurrentIndex)
			{

			}

			const byte& operator*() const
			{
				return mList->Get<byte>(mCurrentIndex);
			}

			const ConstIterator& operator++()
			{
				++mCurrentIndex;
				return *this;
			}

			bool operator!=(const ConstIterator& other) const
			{
				return mCurrentIndex != other.mCurrentIndex;
			}

			const ConstIterator begin() const
			{
				return ConstIterator(mList, 0);
			}

			const ConstIterator end() const
			{
				return ConstIterator(mList, mList->mCount);
			}
		};
	};

	template<typename TResource, typename TInitData>
	struct blListDynamicResource : public blBaseResource
	{
		blListResource mOffsets;

		blListDynamicResource(
			blResourceStream& stream,
			const std::vector<TInitData>& data
		) : blBaseResource(stream), mOffsets(blListResource::New<uint64_t>(stream, data.size()))
		{
			for (uint32_t i = 0; i < data.size(); ++i)
			{
				mOffsets.GetMutable<uint64_t>(i) = stream.GetMemberToDataOffset(this);
				stream.WriteResource<TResource>(data[i]).DataWritten();
			}
		}

		uint32_t GetCount() const
		{
			return mOffsets.mCount;
		}	

		const TResource& Get(uint32_t index) const
		{
			const uint64_t offset = mOffsets.Get<uint64_t>(index);
			return *reinterpret_cast<const TResource*>(reinterpret_cast<const byte*>(this) + offset);
		}

		TResource& GetMutable(uint32_t index)
		{
			return *reinterpret_cast<TResource*>(reinterpret_cast<byte*>(this) + mOffsets.Get<uint64_t>(index));
		}

		TResource& operator[](uint32_t index)
		{
			return GetMutable(index);
		}

		const TResource& operator[](uint32_t index) const
		{
			return Get(index);
		}
		
		class Iterator
		{
		private:
			blListDynamicResource* mList;
			uint32_t mCurrentIndex;
		public:
			Iterator(blListDynamicResource* list, uint32_t currentIndex)
				: mList(list), mCurrentIndex(currentIndex)
			{

			}

			Iterator(blListDynamicResource* list)
				: Iterator(list, 0)
			{

			}

			TResource& operator*() const
			{
				return mList->GetMutable(mCurrentIndex);
			}

			Iterator& operator++()
			{
				++mCurrentIndex;
				return *this;
			}

			bool operator!=(const Iterator& other) const
			{
				return mCurrentIndex != other.mCurrentIndex;
			}

			Iterator begin() const
			{
				return Iterator(mList, 0);
			}

			Iterator end() const
			{
				return Iterator(mList, mList->GetCount());
			}
		};

		class ConstIterator
		{
		private:
			const blListDynamicResource* mList;
			uint32_t mCurrentIndex;
		public:
			ConstIterator(const blListDynamicResource* list, uint32_t currentIndex)
				: mList(list), mCurrentIndex(currentIndex)
			{

			}

			ConstIterator(const blListDynamicResource* list)
				: ConstIterator(list, 0)
			{

			}

			const TResource& operator*() const
			{
				return mList->Get(mCurrentIndex);
			}

			ConstIterator& operator++()
			{
				++mCurrentIndex;
				return *this;
			}

			bool operator!=(const ConstIterator& other) const
			{
				return mCurrentIndex != other.mCurrentIndex;
			}

			ConstIterator begin() const
			{
				return ConstIterator(mList, 0);
			}

			ConstIterator end() const
			{
				return ConstIterator(mList, mList->GetCount());
			}
		};
	};

	struct blStringResource : public blBaseResource
	{
		uint64_t mLength;
		blStringResource(blResourceStream& stream, const std::string& str);
		const std::string_view GetStringView() const;
	};

	struct blWideStringResource : public blBaseResource
	{
		uint64_t mLength;
		blWideStringResource(blResourceStream& stream, const std::wstring& str);
		const std::wstring_view GetStringView() const;
	};
}
