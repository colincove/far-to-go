#pragma once

#include <algorithm>
#include <blMath.inl>

namespace BoulderLeaf::Math
{
	struct Vector2
	{
		Vector2() : 
			Vector2(0, 0)
		{}

		Vector2(float x, float y):
			x(x),
			y(y)
		{}

		union
		{
			struct
			{
				float x, y;
			};

			float data[2];
		};

		const float& operator[](const size_t i) const
		{
			return data[i];
		}

		float& operator[](const size_t i)
		{
			return data[i];
		}

		inline Vector2 Cross(const Vector2& other) const { return Vector2(); }

		inline float Dot(Vector2 other) const
		{ 
			return x * other.x + y * other.y;
		}

		inline Vector2 Normalize() const
		{
			const float magnitude = Magnitude();
			return Vector2(x / magnitude, y / magnitude);
		}

		inline float Magnitude() const
		{
			return sqrtf(x * x + y * y);
		}

		inline bool IsNormalized() const
		{
			return BoulderLeaf::Math::IsNormalized(*this);
		}

		inline bool IsOrthoginal(const Vector2& other) const
		{
			return IsNearZero(Dot(other));
		}

		inline bool IsAngleAcute(const Vector2& other) const
		{
			return Dot(other) > 0;
		}

		inline bool IsAngleObtuse(const Vector2& other) const
		{
			return Dot(other) < 0;
		}

		inline Vector2 ProjectOnto(const Vector2& other)
		{
			return BoulderLeaf::Math::Project<Vector2>(*this, other);
		}

		inline Vector2 Orthogonalize(const Vector2& other)
		{
			return BoulderLeaf::Math::Orthogonalize<Vector2>(*this, other);
		}

		static void Orthogonalize(Vector2* start, Vector2* end)
		{
			BoulderLeaf::Math::Orthogonalize<Vector2>(start, end);
		}

		static bool IsOrthogonalized(const Vector2* start, const Vector2* end)
		{
			return BoulderLeaf::Math::IsOrthogonalized<Vector2>(start, end);
		}
	};

	inline bool operator==(const Vector2& lhs, const Vector2& rhs)
	{
		return std::equal(std::begin(lhs.data), std::end(lhs.data), std::begin(rhs.data));
	}
	
	inline bool operator!=(const Vector2& lhs, const Vector2& rhs) { return !(lhs == rhs); }

	inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs) 
	{
		return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
	}
	
	inline void operator*=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
	}
	
	inline void operator*=(Vector2& lhs, const float& rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
	}
	
	inline Vector2 operator*(const Vector2& lhs, const float& rhs)
	{
		return Vector2(lhs.x * rhs, lhs.y * rhs);
	}
	
	inline Vector2 operator/(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
	}
	
	inline Vector2 operator/(const Vector2& lhs, const float& rhs)
	{
		return Vector2(lhs.x / rhs, lhs.y / rhs);
	}
	
	inline void operator/=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
	}
	
	inline void operator/=(Vector2& lhs, const float& rhs)
	{
		lhs.x /= rhs;
		lhs.y /= rhs;
	}
	
	inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	
	inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	
	inline void operator+=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
	}
	
	inline void operator+=(Vector2& lhs, const float& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
	}
	
	inline void operator-=(Vector2& lhs, const Vector2& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
	}
}