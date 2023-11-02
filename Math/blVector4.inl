#pragma once

#include <algorithm>
#include "blMath.inl"

namespace BoulderLeaf::Math
{
	struct Vector4
	{
		Vector4() :
			Vector4(0, 0, 0, 0)
		{}

		Vector4(float x, float y, float z, float a) :
			x(x),
			y(y),
			z(z), 
			a(a)
		{}

		union
		{
			struct
			{
				float x, y, z, a;
			};

			float data[4];
		};

		const float& operator[](const size_t i) const
		{
			return data[i];
		}

		float& operator[](const size_t i)
		{
			return data[i];
		}

		inline Vector4 Cross(const Vector4& other) const
		{
			return Vector4(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x,
				a * other.a - a * other.a
			);
		}

		inline float Dot(const Vector4& other) const
		{
			return x * other.x + y * other.y + z * other.z + a * other.a;
		}

		inline Vector4 Normalize() const
		{
			const float magnitude = Magnitude();
			return Vector4(x / magnitude, y / magnitude, z / magnitude, a / magnitude);
		}

		inline bool IsNormalized() const
		{
			return IsNearExpected(1, Magnitude());
		}

		inline float Magnitude() const
		{
			return sqrtf(x * x + y * y + z * z + a * a);
		}

		inline bool IsOrthoginal(const Vector4& other) const
		{
			return IsNearZero(Dot(other));
		}

		inline bool IsAngleAcute(const Vector4& other) const
		{
			return Dot(other) > 0;
		}

		inline bool IsAngleObtuse(const Vector4& other) const
		{
			return Dot(other) < 0;
		}

		inline Vector4 ProjectOnto(const Vector4& other)
		{
			return BoulderLeaf::Math::Project<Vector4>(*this, other);
		}
	};


	inline bool operator==(const Vector4& lhs, const Vector4& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	inline bool operator!=(const Vector4& lhs, const Vector4& rhs) { return !(lhs == rhs); }

	inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.a * rhs.a);
	}

	inline void operator*=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		lhs.a *= rhs.a;
	}

	inline void operator*=(Vector4& lhs, const float& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.a *= rhs;
	}

	inline Vector4 operator*(const Vector4& lhs, const float& rhs)
	{
		return Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.a * rhs);
	}

	inline Vector4 operator/(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.a / rhs.a);
	}

	inline Vector4 operator/(const Vector4& lhs, const float& rhs)
	{
		return Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.a / rhs);
	}

	inline void operator/=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		lhs.a /= rhs.a;
	}

	inline void operator/=(Vector4& lhs, const float& rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.a /= rhs;
	}

	inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.a * rhs.a);
	}

	inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.a - rhs.a);
	}

	inline void operator+=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.a += rhs.a;
	}

	inline void operator+=(Vector4& lhs, const float& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;
		lhs.a += rhs;
	}

	inline void operator-=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		lhs.a -= rhs.a;
	}
}