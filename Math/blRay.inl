#pragma once

#include <blVector3.inl>

namespace BoulderLeaf::Math
{
	struct Ray
	{
		Vector3 origin;
		Vector3 direction;

		Ray(Vector3 origin, Vector3 direction) :
			origin(origin),
			direction(direction)
		{};
	};
};