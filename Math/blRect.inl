#pragma once

#include <algorithm>
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

		inline float Left() const { return center.x - halfWidth; }
		inline float Right() const { return center.x + halfWidth; }
		inline float Top() const { return center.y + halfHeight; }
		inline float Bottom() const { return center.y - halfHeight; }

		inline Vector2 TopLeft() const { return Vector2(Left(), Top()); }
		inline Vector2 BottomLeft() const { return Vector2(Left(), Bottom()); }
		inline Vector2 TopRight() const { return Vector2(Right(), Top()); }
		inline Vector2 BottomRight() const { return Vector2(Right(), Bottom()); }

		inline Rect Overlap(Rect other) const 
		{
			const float left = std::max(Left(), other.Left());
			const float right = std::min(Right(), other.Right());
			const float bottom = std::max(Bottom(), other.Bottom());
			const float top = std::min(Top(), other.Top());

			const float halfWidth = (right - left) / 2;
			const float halfHeight = (right - left) / 2;

			return Rect(halfWidth, halfHeight, Vector2(left + halfWidth, bottom + halfHeight));
		}

		inline float Area() const { return halfHeight * halfWidth * 4; }
	};

	inline bool operator==(const Rect& lhs, const Rect& rhs)
	{
		return lhs.halfWidth == rhs.halfWidth && lhs.halfHeight == rhs.halfHeight && lhs.center == rhs.center;
	}

	inline bool operator!=(const Rect& lhs, const Rect& rhs) { return !(lhs == rhs); }
};