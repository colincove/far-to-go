#pragma once

namespace BoulderLeaf
{
	template<typename T>
	class blFlagsOf
	{
	private:
		T mFlags;
	public:
		bool test(T mask) :const
		{
			return (mFlags & mask) == mask;
		}

		void set(T mask, bool value)
		{
			if (value)
			{
				mFlags |= mask;
			}
			else
			{
				mFlags &= ~mask;
			}
		}

		void toggle(T mask)
		{
			mFlags ^= mask;
		}

		void clear()
		{
			mFlags = T(0);
		}

		T get() const
		{
			return mFlags;
		}

		blFlagsOf() : mFlags(T(0)) {}
		blFlagsOf(T flags) : mFlags(flags) {}
		blFlagsOf(const blFlagsOf<T>& other) : mFlags(other.mFlags) {}
		blFlagsOf<T>& operator=(const blFlagsOf<T>& other)
		{
			if (this != &other)
			{
				mFlags = other.mFlags;
			}
			return *this;
		}

		blFlagsOf<T>& operator=(T flags)
		{
			mFlags = flags;
			return *this;
		}

		bool operator==(const blFlagsOf<T>& other) const
		{
			return mFlags == other.mFlags;
		}

		bool operator!=(const blFlagsOf<T>& other) const
		{
			return mFlags != other.mFlags;
		}

		bool operator==(T flags) const
		{
			return mFlags == flags;
		}

		bool operator!=(T flags) const
		{
			return mFlags != flags;
		}

		blFlagsOf<T> operator|(const blFlagsOf<T>& other) const
		{
			return blFlagsOf<T>(mFlags | other.mFlags);
		}

		blFlagsOf<T> operator&(const blFlagsOf<T>& other) const
		{
			return blFlagsOf<T>(mFlags & other.mFlags);
		}

		blFlagsOf<T> operator^(const blFlagsOf<T>& other) const
		{
			return blFlagsOf<T>(mFlags ^ other.mFlags);
		}

		blFlagsOf<T> operator|(T flags) const
		{
			return blFlagsOf<T>(mFlags | flags);
		}

		blFlagsOf<T> operator&(T flags) const
		{
			return blFlagsOf<T>(mFlags & flags);
		}

		blFlagsOf<T> operator^(T flags) const
		{
			return blFlagsOf<T>(mFlags ^ flags);
		}

		blFlagsOf<T>& operator|=(const blFlagsOf<T>& other)
		{
			mFlags |= other.mFlags;
			return *this;
		}

		blFlagsOf<T>& operator&=(const blFlagsOf<T>& other)
		{
			mFlags &= other.mFlags;
			return *this;
		}

		blFlagsOf<T>& operator^=(const blFlagsOf<T>& other)
		{
			mFlags ^= other.mFlags;
			return *this;
		}

		blFlagsOf<T>& operator|=(T flags)
		{
			mFlags |= flags;
			return *this;
		}

		blFlagsOf<T>& operator&=(T flags)
		{
			mFlags &= flags;
			return *this;
		}

		blFlagsOf<T>& operator^=(T flags)
		{
			mFlags ^= flags;
			return *this;
		}

		constexpr operator T() const
		{
			return mFlags;
		}
	};
}