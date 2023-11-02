#pragma once

#include "blVector3.inl"
#include "blPoint3.inl"

namespace BoulderLeaf::Math
{
	struct Ray
	{
		Point3 origin;
		Vector3 direction;

		Ray(Point3 origin, Vector3 direction) :
			origin(origin),
			direction(direction)
		{};
	};
};