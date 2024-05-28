#pragma once
#include <algorithm>
#include "blVector3.inl"
#include "blVector4.inl"
#include "blMatrix4x4.inl"

namespace BoulderLeaf::Math
{
	struct Quaternion
	{
	public:
		union
		{
			float data[4];
			Vector4 vector4;

			struct
			{
				float t;
				float x;
				float y;
				float z;
			};

			struct
			{
				float real; //degree of rotation
				Vector3 vector; //axis of rotation
			};
		};
		
	public:
		Quaternion(float t, float x, float y, float z) :
			t(t),
			x(x),
			y(y),
			z(z)
		{

		}

		Quaternion(float real, Vector3 vector) :
			real(real),
			vector(vector)
		{

		}

		Quaternion():Quaternion(0, 0, 0, 0)
		{

		}

		inline bool IsNormalized() const
		{
			return vector4.IsNormalized();
		}

		inline void NormalizeInPlace()
		{
			vector4 = vector4.Normalize();
		}

		static inline Matrix4x4 RotationMatrix4x4(const Quaternion q)
		{
			assert(q.IsNormalized());
			return Matrix4x4(
				2.0f * (q.real * q.real + q.x * q.x) - 1, 2.0f * (q.x * q.y - q.real * q.z), 2.0f * (q.x * q.z + q.real * q.y), 0,
				2.0f * (q.x * q.y + q.real * q.z), 2.0f * (q.real * q.real + q.y * q.y) - 1.0f, 2.0f * (q.y * q.z - q.real * q.x), 0,
				2.0f * (q.x * q.z - q.real * q.y), 2.0f * (q.y * q.z + q.real * q.x), 2.0f * (q.real * q.real + q.z * q.z) - 1.0f, 0,
				0, 0, 0, 0);
		}

		static inline Matrix3x3 RotationMatrix3x3(const Quaternion q)
		{
			assert(q.IsNormalized());
			return Matrix3x3(
				2.0f * (q.real * q.real + q.x * q.x) - 1, 2.0f * (q.x * q.y - q.real * q.z), 2.0f * (q.x * q.z + q.real * q.y),
				2.0f * (q.x * q.y + q.real * q.z), 2.0f * (q.real * q.real + q.y * q.y) - 1.0f, 2.0f * (q.y * q.z - q.real * q.x),
				2.0f * (q.x * q.z - q.real * q.y), 2.0f * (q.y * q.z + q.real * q.x), 2.0f * (q.real * q.real + q.z * q.z) - 1.0f);
		}
	};

	inline Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
	{
		return Quaternion(
			lhs.t * rhs.t - rhs.vector.Dot(lhs.vector),
			lhs.vector * rhs.t +
			rhs.vector * lhs.t + 
			lhs.vector.Cross(rhs.vector));
	}

	inline bool operator==(const Quaternion& lhs, const Quaternion& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}

	struct UnitQuaternion
	{
	public:
		union
		{
			struct
			{
				Quaternion i;
				Quaternion j;
				Quaternion k;
			};

			Quaternion data[3];
		};

	public:
		UnitQuaternion(Quaternion i, Quaternion j, Quaternion k)
			:i(i), j(j), k(k)
		{

		}
	};

	UnitQuaternion k_unitQuaternions(
		Quaternion(0.0f, 1.0f, 0.0f, 0.0f), 
		Quaternion(0.0f, 0.0f, 1.0f, 0.0f),
		Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
}