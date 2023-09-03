#pragma once

#include <algorithm>
#include <blMath.inl>

namespace BoulderLeaf::Math
{
	struct Vector3
	{
		Vector3() : 
			Vector3(0, 0, 0)
		{}

		Vector3(float x, float y, float z):
			x(x),
			y(y),
			z(z)
		{}

		union
		{
			struct
			{
				float x, y, z;
			};

			float data[3];
		};

		const float& operator[](const size_t i) const
		{
			return data[i];
		}

		float& operator[](const size_t i)
		{
			return data[i];
		}

		inline Vector3 Cross(const Vector3& other) const { return Vector3(); }

		inline float Dot(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}
		
		inline Vector3 Normalize() const
		{ 
			const float magnitude = Magnitude();
			return Vector3(x / magnitude, y / magnitude, z / magnitude);
		}

		inline bool IsNormalized() const
		{
			return IsNearExpected(1, Magnitude());
		}

		inline float Magnitude() const
		{ 
			return sqrtf(x * x + y * y + z * z);
		}

		inline bool IsOrthoginal(const Vector3& other) const
		{
			return IsNearZero(Dot(other));
		}

		inline bool IsAngleAcute(const Vector3& other) const
		{
			return Dot(other) > 0;
		}

		inline bool IsAngleObtuse(const Vector3& other) const
		{
			return Dot(other) < 0;
		}

		inline Vector3 ProjectOnto(const Vector3& other)
		{
			return BoulderLeaf::Math::Project<Vector3>(*this, other);
		}

		inline Vector3 Orthogonalize(const Vector3& other)
		{
			return BoulderLeaf::Math::Orthogonalize<Vector3>(*this, other);
		}

		static void Orthogonalize(Vector3* start)
		{
			Vector3& x1 = *start;
			Vector3& x2 = *(start+1);
			Vector3& x3 = *(start+2);

			x2 = x2.Orthogonalize(x1);
			x3 = x3.Orthogonalize(x1);
			x3 = x3.Orthogonalize(x2);
			x1 = x1.Normalize();
		}

		static bool IsOrthogonalized(const Vector3* start, const Vector3* end)
		{
			return BoulderLeaf::Math::IsOrthogonalized<Vector3>(start, end);
		}
	};

	
	inline bool operator==(const Vector3& lhs, const Vector3& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	inline bool operator!=(const Vector3& lhs, const Vector3& rhs) { return !(lhs == rhs); }
	
	inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs) 
	{
		return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}
	
	inline void operator*=(Vector3& lhs, const Vector3& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
	}
	
	inline void operator*=(Vector3& lhs, const float& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
	}
	
	inline Vector3 operator*(const Vector3& lhs, const float& rhs)
	{
		return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	
	inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
	}
	
	inline Vector3 operator/(const Vector3& lhs, const float& rhs)
	{
		return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	
	inline void operator/=(Vector3& lhs, const Vector3& rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
	}
	
	inline void operator/=(Vector3& lhs, const float& rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
	}
	
	inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	
	inline void operator+=(Vector3& lhs, const Vector3& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
	}
	
	inline void operator+=(Vector3& lhs, const float& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;
	}
	
	inline void operator-=(Vector3& lhs, const Vector3& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
	}
}