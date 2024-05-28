#pragma once

#include <cassert>
#include <cmath>
#include <array>

#include "blMath.inl"
#include "blVector3.inl"

namespace BoulderLeaf::Math
{
	struct CartesianCoordinates
	{
		union
		{
			Vector3 data[3];
			struct
			{
				Vector3 x;
				Vector3 y;
				Vector3 z;
			};
			struct
			{
				Vector3 Forward;
				Vector3 Up;
				Vector3 Left;
			};
		};

		CartesianCoordinates() : CartesianCoordinates(Vector3::Forward(), Vector3::Up(), Vector3::Left()) {}
		CartesianCoordinates(Vector3 x, Vector3 y, Vector3 z)
			:x(x), y(y), z(z)
		{
			assert(x.IsNormalized());
			assert(y.IsNormalized());
			assert(z.IsNormalized());
			assert(Vector3::IsOrthogonalized(std::begin(this->data), std::end(this->data)));
		}

		const Vector3& operator[](const size_t i) const
		{
			return data[i];
		}

		Vector3& operator[](const size_t i)
		{
			return data[i];
		}

		static CartesianCoordinates FromAxes(Vector3 x, Vector3 y, Vector3 z)
		{
			Vector3 a[3](x, y, z);
			Vector3::Orthogonalize(std::begin(a));
			CartesianCoordinates result(a[0], a[1], a[2]);
			return result;
		}
	};

	static CartesianCoordinates AxisStandardBasis;

	inline Vector3 RotateAboutAxisRadians(const Vector3 vector, const Vector3 axis, const float angleRadians)
	{
		assert(axis.IsNormalized());

		const float c(cosf(angleRadians));
		const float s(sinf(angleRadians));

		//this is very close to result. Maybe not entirely correct though?
		return	(vector * c) +
				(axis * ((1.0f - c) * (axis.Dot(vector))) +
				(axis.Cross(vector)) * s);
	}

	inline Vector3 RotateAboutAxisDegrees(const Vector3 vector, const Vector3 axis, const float angleDegrees)
	{
		return RotateAboutAxisRadians(vector, axis, ToRadians(angleDegrees));
	}

	inline Vector3 Transition(
		const Vector3 vector, 
		const CartesianCoordinates frame)
	{
		return frame.x * vector.x + frame.y * vector.y + frame.z * vector.z;
	}

	inline Vector3 Transition(
		const Vector3 vector,
		const CartesianCoordinates coordinates,
		const Vector3 coordinateOrigin)
	{
		return Transition(vector, coordinates) + coordinateOrigin;
	}
}