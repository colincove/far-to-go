#pragma once

#include <blVector3.inl>

namespace BoulderLeaf::Math
{
	struct Quad
	{
		union
		{
			struct
			{
				Vector3 a, b, c, d;
			};

			Vector3 data[4];
		};

		Vector3 center() const;
	};
};