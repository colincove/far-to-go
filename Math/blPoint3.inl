#pragma once

#include "blVector3.inl"

namespace BoulderLeaf::Math
{
	struct Point3
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};

			float data[3];
		};

		Point3(float x, float y, float z) :
			x(x),
			y(y),
			z(z)
		{

		}

		Point3() : Point3(0, 0, 0)
		{

		}

		const float& operator[](const size_t i) const
		{
			return data[i];
		}

		float& operator[](const size_t i)
		{
			return data[i];
		}

		operator Vector3& () { return *reinterpret_cast<Vector3*>(this); }
		operator Vector3() const { return Vector3(x, y, z); }
	};

	inline Vector3 operator-(const Point3& lhs, const Point3& rhs)
	{
		return Vector3(rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z);
	}

	inline Point3 operator+(const Point3& lhs, const Vector3& rhs)
	{
		return Point3(rhs.x + lhs.x, rhs.y + lhs.y, rhs.z + lhs.z);
	}

	inline Point3 operator-(const Point3& lhs, const Vector3& rhs)
	{
		return Point3(rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z);
	}

	inline bool operator==(const Point3& lhs, const Point3& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	inline bool operator!=(const Point3& lhs, const Point3& rhs) { return !(lhs == rhs); }
}