#pragma once

#include <Resources/blResourceHandle.h>

namespace BoulderLeaf
{
	template<typename T>
	class blResourceHandleOfType
	{
		friend blResourceHandle;
	private:
		blResourceHandle mHandle;
	public:
		blResourceHandleOfType() :blResourceHandleOfType(blResourceHandle::Invalid) {}
		blResourceHandleOfType(blResourceHandle handle)
			: mHandle(handle)
		{

		}

		// expose underlying handle for friend accessors
		blResourceHandle GetHandle() const { return mHandle; }

		bool IsValid() const;
		void Release();
		const std::wstring_view GetName() const
		{
			return mHandle.GetName();
		}

		const T* Get() const
		{
			return reinterpret_cast<const T*>(mHandle.Get());
		}

		T* GetMutable()
		{
			return reinterpret_cast<T*>(mHandle.GetMutable());
		}

		T* operator->()
		{
			return reinterpret_cast<T*>(mHandle.GetMutable());
		}

		T& operator*()
		{
			return *reinterpret_cast<T*>(mHandle.GetMutable());
		}

		const T& operator*() const
		{
			return *reinterpret_cast<const T*>(mHandle.Get());
		}

		bool operator!()
		{
			return !mHandle;
		}

		operator bool()
		{
			return mHandle;
		}

		void PushData(T* data)
		{
			mHandle.PushDataOfType<T>(data);
		}

		blResourceContainer* GetContainer() const
		{
			return mHandle.GetContainer();
		}

		blResourceRefOfType<T> GetRef() const
		{
			return blResourceRefOfType<T>(mHandle);
		}
	};

	// Template method implementations must be in the header
	template<typename T>
	bool blResourceHandleOfType<T>::IsValid() const
	{
		return mHandle.IsValid();
	}

	template<typename T>
	void blResourceHandleOfType<T>::Release()
	{
		mHandle.Release();
	}

	template<typename T>
	class blResourceRefOfType
	{
	private:
		blResourceId mId;
	public:
		blResourceRefOfType() : mId()
		{

		}

		blResourceRefOfType(blResourceId id) : mId(id)
		{

		}

		blResourceRefOfType(const blResourceRef& resource) : mId(resource.GetId())
		{

		}

		blResourceId GetId() const
		{
			return mId;
		}

		bool operator==(const blResourceRefOfType& other) const
		{
			return mId == other.mId;
		}

		bool operator!=(const blResourceRefOfType& other) const
		{
			return !(&this == other);
		}

		bool operator!() const
		{
			return !IsValid();
		}

		operator bool() const
		{
			return IsValid();
		}

		bool IsValid() const
		{
			return mId.IsValid();
		}
	};
}