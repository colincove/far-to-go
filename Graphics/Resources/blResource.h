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
		blResourceId() : mId(0) {}

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
		blResourceId(size_t id) : mId(id) {}
	private:
		size_t mId;
	};

	class blResourceCollar
	{
		friend class blResourceManager;
	private:
		blResourceId mId;
		std::shared_ptr<blResourceManager> mOwner;
	public:
		blResourceCollar(blResourceId id, std::shared_ptr<blResourceManager> owner);
		blResourceCollar();
		~blResourceCollar();
	};

	template<typename TData>
	class blResource
	{
		friend class blResourceManager;
	private:
		TData mData;
	public:
		blResource() = default;

		template<class... Types>
		blResource(Types... args) : mData(std::forward<Types>(args)...)
		{
		}

		blResource(const TData& data) : mData(data) {}
		blResource(TData&& data) : mData(std::move(data)) {}
	public:
		const TData& GetData() const
		{
			return mData;
		}
		TData& GetDataMutable()
		{
			return mData;
		}

		operator TData& () { return mData; }
		operator const TData& () const { return mData; }

		TData& GetMutable()
		{
			return mData;
		}

		const TData& Get() const
		{
			return mData;
		}

	private:
		std::optional<blResourceCollar> mCollar;
		std::optional<blResourceCollar>& GetCollarMutable() const
		{
			return *mCollar;
		}
	public:
		const std::optional<blResourceCollar>& GetCollar() const
		{
			return *mCollar;
		}
	};
}