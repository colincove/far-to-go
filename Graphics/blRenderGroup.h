#pragma once

#include <assert.h>
#include <string>

namespace BoulderLeaf::Graphics
{
	struct blRenderGroups;

	struct blRenderGroupData
	{
		std::string Name;
		std::wstring NameWide;
	};

	struct blRenderGroupId
	{
		friend struct blRenderGroups;
		using ValueTypw = unsigned char;
	private:
		constexpr blRenderGroupId(const ValueTypw value) : value(value)
		{
		}
	private:
		ValueTypw value = 0;
	public:
		constexpr operator size_t() const 
		{
			assert(value > 0);
			return value - 1;
		}

		constexpr bool operator==(const blRenderGroupId& rhs)
		{
			return value == rhs.value;
		}

		constexpr bool operator!=(const blRenderGroupId& rhs) { return !(*this == rhs); }

		constexpr bool operator>(const blRenderGroupId& rhs)
		{
			return value > rhs.value;
		}

		constexpr bool operator>=(const blRenderGroupId& rhs)
		{
			return value >= rhs.value;
		}

		constexpr bool operator<(const blRenderGroupId& rhs)
		{
			return value < rhs.value;
		}

		constexpr bool operator<=(const blRenderGroupId& rhs)
		{
			return value <= rhs.value;
		}
	};

	struct blRenderGroups
	{
		friend blRenderGroupId;
	
	public:
		static unsigned char sNextId;
		static constexpr unsigned char MaxValue = 64;
		static constexpr blRenderGroupId Invalid = blRenderGroupId(0);
		static constexpr blRenderGroupId Max = blRenderGroupId(MaxValue);
		static constexpr blRenderGroupId Default = blRenderGroupId(1);
	private:
		static blRenderGroupData Data[MaxValue];
	public:
		blRenderGroups() = delete;

		static size_t GetCount();
		static blRenderGroupId RegisterExternal(std::string name);
		static blRenderGroupData& GetRenderGroupData(blRenderGroupId groupId);

		class Iterator
		{
		private:
			unsigned char currentId;
			Iterator(unsigned char start) : currentId(start) {}
		public:
			Iterator() : currentId(1) {}

			blRenderGroupId operator*() const
			{
				return blRenderGroupId(currentId);
			}

			Iterator& operator++()
			{
				assert(currentId < blRenderGroups::MaxValue);
				++currentId;
				return *this;
			}

			bool operator!=(const Iterator& other) const
			{
				return currentId != other.currentId;
			}

			Iterator begin() const { return Iterator(1); }
			Iterator end() const { return Iterator(sNextId + 2); }
		};
	};
}

#define BL_STATIC_RENDER_GROUP(Name, NameStringMeta) static const blRenderGroupId Name = BoulderLeaf::Graphics::blRenderGroups::RegisterExternal(NameStringMeta)