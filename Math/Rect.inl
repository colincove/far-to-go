#include "Vector3.inl";

namespace BoulderLeaf::Math::Templates
{
	template<typename T>
	struct Rect
	{
		T halfWidth, halfHeight;
		Vector3<T> center;

		Rect(T halfWidth, T halfHeight, Vector3<T> center):
			halfWidth(halfWidth),
			halfHeight(halfHeight),
			center(center)
		{};

		inline T left() const;
		inline T right() const;
		inline T top() const;
		inline T bottom() const;

		inline Vector3<T> topLeft() const;
		inline Vector3<T> bottomLeft() const;
		inline Vector3<T> topRight() const;
		inline Vector3<T> bottomRight() const;
	};
};