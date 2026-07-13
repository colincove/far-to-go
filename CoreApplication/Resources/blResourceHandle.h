#pragma once

#include <cstdint>
#include <Resources/blResourceGuid.h>
#include <string>

namespace BoulderLeaf
{
	using resource_container_id = unsigned char;
	class blResourceContainer;
	class blResourceRef;

	template<typename T>
	class blResourceHandleOfType;

	template<typename T>
	class blResourceRefOfType;

	struct blResourceId
	{
		// TODO: I did not want these to be seperate value
		// write code that combines index and container id into same uint32_t
		// same with mIsValid. Maybe shove that into a single bit in uint32_t mValue
		uint32_t mValue;
		resource_container_id mContainerId;
		bool mIsValid = false;

		constexpr blResourceId();
		constexpr blResourceId(resource_container_id containerId, uint32_t value);

		static blResourceId Invalid;

		resource_container_id GetContainerId() const;
		uint32_t GetIndex() const;

		bool IsValid() const;
	};

	class blResourceHandle
	{
		friend blResourceRef;
		friend blResourceContainer;
	private:
		blResourceId mId;
		blResourceContainer* mOwner;

		constexpr blResourceHandle(blResourceId mId, blResourceContainer* owner);

	public:
		constexpr blResourceHandle();
		constexpr blResourceHandle(const blResourceHandle& other)
			: blResourceHandle(other.mId, other.mOwner)
		{

		}

		template<typename TResource>
		constexpr blResourceHandle(const blResourceHandleOfType<TResource>& resourceOfType)
			: mId(resourceOfType.mHandle.mId), mOwner(resourceOfType.mHandle.mOwner)
		{

		}

		void PushData(byte* data, uint64_t size);

		template<typename TData>
		void PushDataOfType(TData* data)
		{
			PushData(reinterpret_cast<byte*>(data), sizeof(TData));
		}

		static blResourceHandle Invalid;

		void Release();
		bool IsValid() const;
		const blResourceGuid GetGuid() const;
		const blResourceId& GetId() const;
		const std::wstring_view GetName() const;
		blResourceContainer* GetContainer() const;

		const byte* Get() const;
		byte* GetMutable();

		byte* operator->();
		byte& operator*();
		bool operator!();
		operator bool();
	};

	class blResourceRef
	{
	private:
		blResourceId mId;
	public:
		blResourceRef();
		blResourceRef(blResourceId id);

		blResourceRef(const blResourceHandle& resource);

		template<typename TResource>
		constexpr blResourceRef(const blResourceHandleOfType<TResource>& resourceOfType)
			: blResourceRef(resourceOfType.GetHandle().mId)
		{

		}

		template<typename TResource>
		constexpr blResourceRef(const blResourceRefOfType<TResource>& resourceRefOfType)
			: blResourceRef(resourceRefOfType.GetId())
		{

		}

		blResourceId GetId() const;

		bool operator==(const blResourceRef& other) const;

		bool operator!=(const blResourceRef& other) const;

		bool operator!() const;
		operator bool() const;

		bool IsValid() const;
	};
}