#pragma once

#include <algorithm>

namespace BoulderLeaf::Math
{
	template<typename T>
	struct Vector3
	{
		Vector3() : 
			Vector3(0, 0, 0)
		{}

		Vector3(T x, T y, T z):
			x(x),
			y(y),
			z(z)
		{}

		union
		{
			struct
			{
				T x, y, z;
			};

			T data[3];
		};

		const T& operator[](const size_t i) const
		{
			return data[i];
		}

		T& operator[](const size_t i)
		{
			return data[i];
		}

		inline Vector3<T> Cross(Vector3<T> other) { return Vector3<T>(); }
		inline T Dot(Vector3<T> other) { return 0; }
		inline Vector3<T> Normalize() { return Vector3<T>(); }
		inline T Magnitude() { return 0; }
	};

	template<typename T>
	inline bool operator==(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	template<typename T>
	inline bool operator!=(const Vector3<T>& lhs, const Vector3<T>& rhs) { return !(lhs == rhs); }

	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& lhs, const Vector3<T>& rhs) 
	{
		return Vector3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	template<typename T>
	inline void operator*=(Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
	}

	template<typename T>
	inline void operator*=(Vector3<T>& lhs, const T& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
	}

	template<typename T>
	inline Vector3<T> operator*(const Vector3<T>& lhs, const T& rhs)
	{
		return Vector3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}

	template<typename T>
	inline Vector3<T> operator/(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
	}

	template<typename T>
	inline Vector3<T> operator/(const Vector3<T>& lhs, const T& rhs)
	{
		return Vector3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}

	template<typename T>
	inline void operator/=(Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
	}

	template<typename T>
	inline void operator/=(Vector3<T>& lhs, const T& rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
	}

	template<typename T>
	inline Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	template<typename T>
	inline Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template<typename T>
	inline void operator+=(Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
	}

	template<typename T>
	inline void operator+=(Vector3<T>& lhs, const T& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;
	}

	template<typename T>
	inline void operator-=(Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
	}
}