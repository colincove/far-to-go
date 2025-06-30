#pragma once
#include <algorithm>
#include <array>
#include "blVector3.inl"
#include "blVector4.inl"
#include "blMatrix4x4.inl"
#include "blMath.inl"
#include <cmath>

namespace BoulderLeaf::Math
{
	struct Quaternion
	{
	public:

		static const size_t k_size = 4;

		union
		{
			float data[k_size];
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
		constexpr Quaternion(float t, float x, float y, float z) :
			t(t),
			x(x),
			y(y),
			z(z)
		{

		}

		constexpr Quaternion(float real, Vector3 vector) :
			real(real),
			vector(vector)
		{

		}

		constexpr Quaternion():Quaternion(0, 0, 0, 0)
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

		inline Vector3 ToEuler() const
		{
			// roll (x-axis rotation)
			const float sinr_cosp = 2 * (t * x + y * z);
			const float cosr_cosp = 1 - 2 * (x * x + y * y);

			// pitch (y-axis rotation)
			const float sinp = std::sqrt(1 + 2 * (t * y - x * z));
			const float cosp = std::sqrt(1 - 2 * (t * y - x * z));

			// yaw (z-axis rotation)
			const float siny_cosp = 2 * (t * z + x * y);
			const float cosy_cosp = 1 - 2 * (y * y + z * z);

			return Vector3(
				std::atan2f(sinr_cosp, cosr_cosp),
				2 * std::atan2f(sinp, cosp) - PIf / 2,
				std::atan2f(siny_cosp, cosy_cosp)
			);
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

		static inline Quaternion FromEuler(const Vector3 eulerAngles)
		{
			const float cr = cosf(eulerAngles.x * 0.5f);
			const float sr = sinf(eulerAngles.x * 0.5f);
			const float cp = cosf(eulerAngles.y * 0.5f);
			const float sp = sinf(eulerAngles.y * 0.5f);
			const float cy = cosf(eulerAngles.z * 0.5f);
			const float sy = sinf(eulerAngles.z * 0.5f);

			return Quaternion(
				cr * cp * cy + sr * sp * sy,
				sr * cp * cy - cr * sp * sy,
				cr * sp * cy + sr * cp * sy,
				cr * cp * sy - sr * sp * cy
			);
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
}