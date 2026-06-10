#pragma once

#include <optional>
#include <memory>

namespace BoulderLeaf::Graphics
{
	class blResourceManager;

	class blResourceId
	{
		friend class blResourceManager;
	public:
		blResourceId() : mId(0), mInitialized(false) {}

		bool operator<(const blResourceId& other) const
		{
			return mId < other.mId;
		}

		bool operator>(const blResourceId& other) const
		{
			return mId > other.mId;
		}

		bool operator==(const blResourceId& other) const
		{
			return mId == other.mId;
		}

		bool operator!=(const blResourceId& other) const
		{
			return !(*this == other);
		}

	private:
		blResourceId(size_t id) : blResourceId(id, true){}
		blResourceId(size_t id, bool initialized) : mId(id), mInitialized(initialized) {}
	private:
		size_t mId;
		bool mInitialized;
	public:
		size_t GetRaw() const { return mId; }
		bool IsValid() const { return mInitialized; }

		static blResourceId Invalid;
	};

	enum class eResourceType
	{
		Scene,
		Mesh,
		Shader,
		Material,
		DataBuffer,
		Count
	};

	class blResourceBase
	{
	protected:
		blResourceId mId;
	public:
		blResourceBase(blResourceId id) :mId(id) {}
		const blResourceId& GetId() const
		{
			return mId;
		}
	};

	using blResourceBasePtr = std::shared_ptr<blResourceBase>;

	template<typename TData, eResourceType TType>
	class blResource : public blResourceBase
	{
		friend class blResourceManager;
	private:
		TData mData;
	public:
		using DataType = TData;
		constexpr static eResourceType ResourceType = TType;
		blResource() = delete;

		template<class... Types>
		blResource(blResourceId id, Types... args) : mData(std::forward<Types>(args)...),
			blResourceBase(id)
		{
		}

		blResource(blResourceId id, const TData& data) : mData(data), blResourceBase(id) {}
		blResource(blResourceId id, TData&& data) : mData(std::move(data)), blResourceBase(id) {}
	public:
		const TData& GetData() const
		{
			return mData;
		}

		TData& GetDataMutable()
		{
			return mData;
		}
	};

	template<typename TResource>
	using blResourcePtr = std::shared_ptr<TResource>;

	template<typename TResource>
	using blResourceWeakPtr = std::weak_ptr<TResource>;

	template<typename TResource>
	using blResourceUniquePtr = std::unique_ptr<TResource>;
}

#include <functional>

#define BL_RESOURCE_NAMESPACE() BoulderLeaf::Graphics

#define BL_RESOURCE_TYPE_NAME(TData) \
TData##Resource

#define BL_RESOURCE_TYPE_NAME_QUALIFIED(TData) \
BL_RESOURCE_NAMESPACE()##::##TData##Resource

#define BL_RESOURCE_PTR_TYPE_NAME(TData) \
TData##ResourcePtr

#define BL_RESOURCE_PTR_TYPE_NAME_QUALIFIED(TData) \
BL_RESOURCE_NAMESPACE()##::##TData##ResourcePtr

#define BL_RESOURCE_WEAK_PTR_TYPE_NAME(TData) \
TData##ResourceWeakPtr

#define BL_RESOURCE_UNIQUE_PTR_TYPE_NAME(TData) \
TData##ResourceUniquePtr

#define BL_RESOURCE_WEAK_PTR_TYPE_NAME_QUALIFIED(TData) \
BL_RESOURCE_NAMESPACE()##::##TData##ResourceWeakPtr

#define BL_RESOURCE_LESS_THAN_DECL(TData)\
namespace std { \
	bool operator<(const BL_RESOURCE_TYPE_NAME_QUALIFIED(TData)& a, const BL_RESOURCE_TYPE_NAME_QUALIFIED(TData)& b) noexcept; \
}

#define BL_RESOURCE_LESS_THAN_IMPL(TData)\
namespace std { \
	bool operator<(const BL_RESOURCE_TYPE_NAME_QUALIFIED(TData)& a, const BL_RESOURCE_TYPE_NAME_QUALIFIED(TData)& b) noexcept { \
		return a.GetId().GetRaw() < b.GetId().GetRaw(); \
	} \
}

#define BL_RESOURCE_HASH(TData)\
namespace std { \
	template<> \
	struct hash<BL_RESOURCE_TYPE_NAME_QUALIFIED(TData)> { \
		size_t operator()(const BL_RESOURCE_TYPE_NAME_QUALIFIED(TData)& resource) const noexcept { \
			return resource.GetId().GetRaw(); \
		} \
	}; \
}

#define BL_RESOURCE_POINTERS(TData, TResourceType, Name) \
using BL_RESOURCE_PTR_TYPE_NAME(Name) = blResourcePtr<BL_RESOURCE_TYPE_NAME(Name)>; \
using BL_RESOURCE_WEAK_PTR_TYPE_NAME(Name) = blResourceWeakPtr<BL_RESOURCE_TYPE_NAME(Name)>; \
using BL_RESOURCE_UNIQUE_PTR_TYPE_NAME(Name) = blResourceUniquePtr<BL_RESOURCE_TYPE_NAME(Name)>;\
using TData##WeakPtr = std::weak_ptr<TData>;\
using TData##Ptr = std::shared_ptr<TData>;\
using TData##UniquePtr = std::unique_ptr<TData>;

#define BL_RESOURCE_WITH_NAME(TData, TResourceType, Name) \
namespace BL_RESOURCE_NAMESPACE() { \
using BL_RESOURCE_TYPE_NAME(Name) = blResource<TData, TResourceType>; \
BL_RESOURCE_POINTERS(TData, TResourceType, Name) \
} \
BL_RESOURCE_HASH(Name) \
BL_RESOURCE_LESS_THAN_DECL(Name)

#define BL_RESOURCE_IMPL(TData) \
BL_RESOURCE_LESS_THAN_IMPL(TData)

#define BL_RESOURCE(TData, TResourceType) \
BL_RESOURCE_WITH_NAME(TData, TResourceType, TData)

#define BL_DERIVED_RESOURCE(TData, TResourceType) \
BL_RESOURCE_POINTERS(TData, TResourceType, TData)