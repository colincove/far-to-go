#pragma once

#include <algorithm>
#include <cstdlib>

namespace BoulderLeaf::Math::Templates
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

		inline Vector3<T> Cross(const Vector3<T>& other) const { return Vector3<T>(); }

		inline T Dot(const Vector3<T>& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}
		
		inline Vector3<T> Normalize() const
		{ 
			const T magnitude = Magnitude();
			return Vector3<T>(x / magnitude, y / magnitude, z / magnitude);
		}

		inline T Magnitude() const
		{ 
			return std::sqrtf(x * x + y * y + z * z);
		}

		inline bool IsOrthoginal(const Vector3<T>& other) const
		{
			return Dot(other) == 0;
		}

		inline bool IsAngleAcute(const Vector3<T>& other) const
		{
			return Dot(other) > 0;
		}

		inline bool IsAngleObtuse(const Vector3<T>& other) const
		{
			return Dot(other) < 0;
		}
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