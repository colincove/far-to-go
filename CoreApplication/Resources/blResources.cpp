#include <Resources/blResourcesExprimental.h>
#include <blCore.h>
#include <Resources/blResourceGuid.h>

namespace
{
	BOOL IsEmptyGuid(const GUID* pGuid)
	{
		return (pGuid->Data1 == 0) &&
			(pGuid->Data2 == 0) &&
			(pGuid->Data3 == 0) &&
			(*(DWORD*)pGuid->Data4 == 0) && (*(DWORD*)(pGuid->Data4 + 4) == 0);
	}
}

namespace BoulderLeaf
{
	blResourceGuid blResourceGuid::New()
	{
		blResourceGuid guid;
		HRESULT result = CoCreateGuid(&guid.mValue);
		return guid;
	}

	bool blResourceGuid::IsValid() const
	{
		return !IsEmptyGuid(&mValue);
	}

	void blResourceGuid::Clear()
	{
		mValue = GUID();
	}

	bool blResourceGuid::operator==(const blResourceGuid& other) const
	{
		return other.mValue == mValue;
	}

	constexpr blResourceId::blResourceId()
		: mValue(0), mContainerId(0), mIsValid(false)
	{

	}

	constexpr blResourceId::blResourceId(resource_container_id containerId, uint32_t value)
		: mValue(value), mContainerId(containerId), mIsValid(true)
	{

	}

	blResourceId blResourceId::Invalid = blResourceId();

	resource_container_id blResourceId::GetContainerId() const
	{
		return mContainerId;
	}

	uint32_t blResourceId::GetIndex() const
	{
		return mValue;
	}

	bool blResourceId::IsValid() const
	{
		return mIsValid;
	}

	// Empty stub implementations
	blResourceContainer::blResourceContainer(
		resource_container_id id,
		uint64_t heapSize,
		uint32_t numberOfEntries,
		blResourceContainerPool* owningPool)
		: mRegistry(std::make_unique<Entry[]>(numberOfEntries)), 
		mHeap(std::make_unique<byte[]>(heapSize)),
		mId(id), 
		owningPool(owningPool),
		mHeapEnd(0),
		mNumberOfEntries(numberOfEntries)
	{
	}

	void blResourceContainer::Defrag()
	{
		//TODO
	}

	void blResourceContainer::Resize(uint64_t heapSize)
	{
		const uint64_t currentSize = sizeof(mHeap.get());
		if (heapSize > currentSize)
		{
			const std::unique_ptr<byte[]> tmp(std::move(mHeap));
			mHeap = std::make_unique<byte[]>(heapSize);
			memcpy(mHeap.get(), tmp.get(), currentSize);
		}
	}

	void blResourceContainer::Release()
	{
		//TODO
	}

	void blResourceContainer::Release(blResourceId id)
	{
		Entry& entry = *(mRegistry.get() + id.GetIndex());
		entry.ReleaseResource();
	}

	blResourceHandle blResourceContainer::CreateResource(const std::wstring& name, uint64_t size)
	{
		uint32_t index = 0;

		for (Entry* entry = mRegistry.get();
			entry != mRegistry.get() + mNumberOfEntries;
			++entry)
		{
			//TODO if IsMemoryFragment is true, and it has enough space, we should re-allocate the entry. 
			//Use it for this new allocation.
			//NOTE: If I do this. would I then need to create a new Entry that describes the empty space created?
			//eg. if I allocate something of size 10 into a fragmented memory space of size 15,
			//then I need to describe the remaining 5 as an empty space. As an entry.
			//this is because I intend to use these entries to de-fragment the memory in the future. 
			//so it must be described.
			if (entry->IsAvailableForAllocation())
			{
				entry->mGuid = blResourceGuid::New();
				entry->mSize = size;
				entry->mOffset = mHeapEnd;//offset into heap
				entry->mNameLength = name.length(); //TODO

				const uint64_t sizeOfName = name.length() * sizeof(wchar_t);
				memcpy(mHeap.get() + (mHeapEnd + size), name.c_str(), sizeOfName);
				mHeapEnd += size + sizeOfName;
				break;
			}
			else
			{
				++index;
			}
		}

		return CreateResourceWithGuid(name, blResourceGuid::New(), size);
	}

	blResourceHandle blResourceContainer::CreateResourceWithGuid(const std::wstring& name, blResourceGuid guid, uint64_t size)
	{
		uint32_t index = 0;

		for (Entry* entry = mRegistry.get();
			entry != mRegistry.get() + mNumberOfEntries;
			++entry)
		{
			//TODO if IsMemoryFragment is true, and it has enough space, we should re-allocate the entry. 
			//Use it for this new allocation.
			//NOTE: If I do this. would I then need to create a new Entry that describes the empty space created?
			//eg. if I allocate something of size 10 into a fragmented memory space of size 15,
			//then I need to describe the remaining 5 as an empty space. As an entry.
			//this is because I intend to use these entries to de-fragment the memory in the future. 
			//so it must be described.
			if (entry->IsAvailableForAllocation())
			{
				entry->mGuid = guid;
				entry->mSize = size;
				entry->mOffset = mHeapEnd;//offset into heap
				entry->mNameLength = name.length(); //TODO

				const uint64_t sizeOfName = name.length() * sizeof(wchar_t);
				memcpy(mHeap.get() + (mHeapEnd + size), name.c_str(), sizeOfName);
				mHeapEnd += size + sizeOfName;
				break;
			}
			else
			{
				++index;
			}
		}

		return blResourceHandle(blResourceId(mId, index), this);
	}

	const byte* blResourceContainer::GetData(blResourceId id) const
	{
		const Entry& entry = mRegistry.get()[id.GetIndex()];
		return mHeap.get() + entry.mOffset;
	}

	byte* blResourceContainer::GetDataMutable(blResourceId id)
	{
		const Entry& entry = mRegistry.get()[id.GetIndex()];
		return mHeap.get() + entry.mOffset;
	}

	const blResourceGuid& blResourceContainer::GetDataGuid(blResourceId id) const
	{
		const Entry& entry = mRegistry.get()[id.GetIndex()];
		return entry.mGuid;
	}

	const std::wstring_view blResourceContainer::GetDataName(blResourceId id) const
	{
		const Entry& entry = mRegistry.get()[id.GetIndex()];
		const byte* start = GetData(id);
		const wchar_t* stringStart = reinterpret_cast<const wchar_t*>(start + entry.mSize);
		return std::wstring_view(stringStart, entry.mNameLength);
	}

	uint32_t blResourceContainer::Capacity() const
	{
		return mNumberOfEntries;
	}

	blResourceHandle blResourceContainer::FindByGuid(blResourceGuid& guid)
	{
		uint32_t index = 0;
		for (Entry* entry = mRegistry.get();
			entry != mRegistry.get() + mNumberOfEntries;
			++entry)
		{
			if (entry->mGuid == guid)
			{
				return blResourceHandle(blResourceId(mId, index), this);
			}

			++index;
		}

		return blResourceHandle();
	}

	blResourceContainerPool::blResourceContainerPool()
		: mCurrentId(0)
	{

	}

	blResourceContainer* blResourceContainerPool::GetContainer(
		uint64_t heapSize,
		uint32_t numberOfEntries)
	{
		return new blResourceContainer(++mCurrentId, heapSize, numberOfEntries, this);
	}

	void blResourceContainerPool::Return(const blResourceContainer& container)
	{
		//TODO
	}

	bool blResourceContainer::Entry::IsAvailableForAllocation() const
	{
		return mSize == 0 && !mGuid.IsValid();
	}

	void blResourceContainer::Entry::ReleaseResource()
	{
		mGuid.Clear();
	}

	bool blResourceContainer::Entry::IsMemoryFragment() const
	{
		return mSize > 0 && !mGuid.IsValid();
	}

	constexpr blResourceHandle::blResourceHandle()
		: blResourceHandle(blResourceId(), nullptr)
	{
	}

	constexpr blResourceHandle::blResourceHandle(blResourceId id, blResourceContainer* owner)
		: mId(id), mOwner(owner)
	{
	}

	blResourceHandle blResourceHandle::Invalid = blResourceHandle();

	void blResourceHandle::PushData(byte* data, uint64_t size)
	{
		memcpy(GetMutable(), data, size);
	}

	void blResourceHandle::Release()
	{
		mOwner = nullptr;
		mId = blResourceId::Invalid;
	}

	bool blResourceHandle::IsValid() const
	{
		return mOwner != nullptr && mId.IsValid();
	}

	const blResourceGuid blResourceHandle::GetGuid() const
	{
		return mOwner->GetDataGuid(mId);
	}

	const blResourceId& blResourceHandle::GetId() const 
	{
		return mId;
	}

	const std::wstring_view blResourceHandle::GetName() const
	{
		return mOwner->GetDataName(mId);
	}

	blResourceContainer* blResourceHandle::GetContainer() const
	{
		return mOwner;
	}

	const byte* blResourceHandle::Get() const
	{
		return mOwner->GetData(mId);
	}

	byte* blResourceHandle::GetMutable()
	{
		return mOwner->GetDataMutable(mId);
	}

	byte* blResourceHandle::operator->()
	{
		return GetMutable();
	}

	byte& blResourceHandle::operator*()
	{
		return *GetMutable();
	}

	bool blResourceHandle::operator!()
	{
		return !IsValid();
	}

	blResourceHandle::operator bool()
	{
		return IsValid();
	}
}