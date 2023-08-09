#include <blVector3.inl>

namespace BoulderLeaf::Math::Templates
{
	template<typename T>
	struct Ray
	{
		Vector3<T> origin;
		Vector3<T> direction;

		Ray(Vector3<T> origin, Vector3<T> direction) :
			origin(origin),
			direction(direction)
		{};
	};
};