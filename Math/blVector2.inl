#pragma once

#include <algorithm>

namespace BoulderLeaf::Math::Templates
{
	template<typename T>
	struct Vector2
	{
		Vector2() : 
			Vector2(0, 0)
		{}

		Vector2(T x, T y):
			x(x),
			y(y)
		{}

		union
		{
			struct
			{
				T x, y;
			};

			T data[2];
		};

		const T& operator[](const size_t i) const
		{
			return data[i];
		}

		T& operator[](const size_t i)
		{
			return data[i];
		}

		inline Vector2<T> Cross(const Vector2<T>& other) const { return Vector2<T>(); }

		inline T Dot(Vector2<T> other) const 
		{ 
			return x * other.x + y * other.y;
		}

		inline Vector2<T> Normalize() const
		{
			const T magnitude = Magnitude();
			return Vector2<T>(x / magnitude, y / magnitude);
		}

		inline T Magnitude() const
		{ 
			return std::sqrtf(x * x + y * y);
		}

		inline bool IsOrthoginal(const Vector2<T>& other) const
		{
			return Dot(other) == 0;
		}

		inline bool IsAngleAcute(const Vector2<T>& other) const
		{
			return Dot(other) > 0;
		}

		inline bool IsAngleObtuse(const Vector2<T>& other) const
		{
			return Dot(other) < 0;
		}
	};

	template<typename T>
	inline bool operator==(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	template<typename T>
	inline bool operator!=(const Vector2<T>& lhs, const Vector2<T>& rhs) { return !(lhs == rhs); }

	template<typename T>
	inline Vector2<T> operator*(const Vector2<T>& lhs, const Vector2<T>& rhs) 
	{
		return Vector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	template<typename T>
	inline void operator*=(Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
	}

	template<typename T>
	inline void operator*=(Vector2<T>& lhs, const T& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
	}

	template<typename T>
	inline Vector2<T> operator*(const Vector2<T>& lhs, const T& rhs)
	{
		return Vector2<T>(lhs.x * rhs, lhs.y * rhs);
	}

	template<typename T>
	inline Vector2<T> operator/(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
	}

	template<typename T>
	inline Vector2<T> operator/(const Vector2<T>& lhs, const T& rhs)
	{
		return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
	}

	template<typename T>
	inline void operator/=(Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
	}

	template<typename T>
	inline void operator/=(Vector2<T>& lhs, const T& rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
	}

	template<typename T>
	inline Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template<typename T>
	inline Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template<typename T>
	inline void operator+=(Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
	}

	template<typename T>
	inline void operator+=(Vector2<T>& lhs, const T& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
	}

	template<typename T>
	inline void operator-=(Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
	}
}