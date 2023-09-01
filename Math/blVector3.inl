#pragma once

#include <algorithm>
#include <cstdlib>

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

		inline float Magnitude() const
		{ 
			return sqrtf(x * x + y * y + z * z);
		}

		inline bool IsOrthoginal(const Vector3& other) const
		{
			return Dot(other) == 0;
		}

		inline bool IsAngleAcute(const Vector3& other) const
		{
			return Dot(other) > 0;
		}

		inline bool IsAngleObtuse(const Vector3& other) const
		{
			return Dot(other) < 0;
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