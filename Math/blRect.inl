#include <blVector2.inl>

namespace BoulderLeaf::Math::Templates
{
	template<typename T>
	struct Rect
	{
		T halfWidth, halfHeight;
		Vector2<T> center;

		Rect() : Rect(0, 0) {};
		Rect(T halfWidth, T halfHeight) : Rect(halfWidth, halfHeight, Vector2<T>()) {};
		Rect(T halfWidth, T halfHeight, Vector2<T> center):
			halfWidth(halfWidth),
			halfHeight(halfHeight),
			center(center)
		{};

		inline T Left() const { return 0; }
		inline T Right() const { return 0; }
		inline T Top() const { return 0; }
		inline T Bottom() const { return 0; }

		inline Vector2<T> TopLeft() const { return Vector2<T>(); }
		inline Vector2<T> BottomLeft() const { return Vector2<T>(); }
		inline Vector2<T> TopRight() const { return Vector2<T>(); }
		inline Vector2<T> BottomRight() const { return Vector2<T>(); }

		inline Rect Overlap(Rect other) const { return Rect(); }
		inline T Area() const { return 0; }
	};

	template<typename T>
	inline bool operator==(const Rect<T>& lhs, const Rect<T>& rhs)
	{
		return lhs.halfWidth == rhs.halfWidth && lhs.halfHeight == rhs.halfHeight && lhs.center == rhs.center;
	}

	template<typename T>
	inline bool operator!=(const Rect<T>& lhs, const Rect<T>& rhs) { return !(lhs == rhs); }
};