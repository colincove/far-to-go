#include <blVector2.inl>

namespace BoulderLeaf::Math
{
	struct Rect
	{
		float halfWidth, halfHeight;
		Vector2 center;

		Rect() : Rect(0, 0) {};
		Rect(float halfWidth, float halfHeight) : Rect(halfWidth, halfHeight, Vector2()) {};
		Rect(float halfWidth, float halfHeight, Vector2 center):
			halfWidth(halfWidth),
			halfHeight(halfHeight),
			center(center)
		{};

		inline float Left() const { return 0; }
		inline float Right() const { return 0; }
		inline float Top() const { return 0; }
		inline float Bottom() const { return 0; }

		inline Vector2 TopLeft() const { return Vector2(); }
		inline Vector2 BottomLeft() const { return Vector2(); }
		inline Vector2 TopRight() const { return Vector2(); }
		inline Vector2 BottomRight() const { return Vector2(); }

		inline Rect Overlap(Rect other) const { return Rect(); }
		inline float Area() const { return halfHeight * halfWidth * 4; }
	};

	inline bool operator==(const Rect& lhs, const Rect& rhs)
	{
		return lhs.halfWidth == rhs.halfWidth && lhs.halfHeight == rhs.halfHeight && lhs.center == rhs.center;
	}

	inline bool operator!=(const Rect& lhs, const Rect& rhs) { return !(lhs == rhs); }
};