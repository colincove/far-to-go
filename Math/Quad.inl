#include "Vector3.inl";

namespace BoulderLeaf::Math::Templates
{
	template<typename T>
	struct Quad
	{
		union
		{
			struct
			{
				Vector3<T> a, b, c, d;
			};

			Vector3<T> data[4];
		};

		Vector3<T> center() const;
	};
};