#pragma once

#include <algorithm>
#include "blMath.inl"
#include "blVector3.inl"

namespace BoulderLeaf::Math
{
	struct Vector4
	{
		Vector4() :
			Vector4(0, 0, 0, 0)
		{}

		Vector4(float x, float y, float z, float w) :
			x(x),
			y(y),
			z(z), 
			w(w)
		{}

		union
		{
			struct
			{
				float x, y, z, w;
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
				w * other.w - w * other.w
			);
		}

		inline float Dot(const Vector4& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		inline Vector4 Normalize() const
		{
			const float magnitude = Magnitude();
			return Vector4(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
		}

		inline bool IsNormalized() const
		{
			return IsNearEqual(1, Magnitude());
		}

		inline float Magnitude() const
		{
			return sqrtf(x * x + y * y + z * z + w * w);
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

		static inline Vector4 Point3D(const float x, const float y, const float z)
		{
			return Vector4(x, y, z, 1);
		}

		static inline Vector4 Vector3D(const float x, const float y, const float z)
		{
			return Vector4(x, y, z, 0);
		}

		static inline Vector4 Vector3D(const Vector3 vector)
		{
			return Vector3D(vector.x, vector.y, vector.z);
		}

		static inline Vector4 Point3D(const Vector3 vector)
		{
			return Point3D(vector.x, vector.y, vector.z);
		}
 	};


	inline bool operator==(const Vector4& lhs, const Vector4& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	inline bool operator!=(const Vector4& lhs, const Vector4& rhs) { return !(lhs == rhs); }

	inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	}

	inline void operator*=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		lhs.w *= rhs.w;
	}

	inline void operator*=(Vector4& lhs, const float& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.w *= rhs;
	}

	inline Vector4 operator*(const Vector4& lhs, const float& rhs)
	{
		return Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}

	inline Vector4 operator/(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
	}

	inline Vector4 operator/(const Vector4& lhs, const float& rhs)
	{
		return Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
	}

	inline void operator/=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		lhs.w /= rhs.w;
	}

	inline void operator/=(Vector4& lhs, const float& rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.w /= rhs;
	}

	inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w * rhs.w);
	}

	inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	inline void operator+=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.w += rhs.w;
	}

	inline void operator+=(Vector4& lhs, const float& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;
		lhs.w += rhs;
	}

	inline void operator-=(Vector4& lhs, const Vector4& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		lhs.w -= rhs.w;
	}

	inline bool NearEqual(const Vector4& a, const Vector4& b)
	{
		return IsNearEqual(a.x, b.x) && IsNearEqual(a.y, b.y) && IsNearEqual(a.z, b.z) && IsNearEqual(a.w, b.w);
	}
}