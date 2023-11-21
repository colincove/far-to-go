#pragma once

#include <cassert>
#include <cmath>

#include "blMath.inl"
#include "blVector3.inl"

namespace BoulderLeaf::Math
{
	static Vector3 AxisStandardBasis[]
	{
		Vector3(1, 0, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};

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
}