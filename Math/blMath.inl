#pragma once
#include <cmath>
namespace BoulderLeaf::Math
{
	#define PIf 3.14159265f

	static float Epsilon = 0.000001f;

	inline bool IsNearEqual(float expected, float actual)
	{
		return fabs(expected - actual) < Epsilon;
	}

	inline bool IsNearEqual(const float* startA, const float* startB, size_t size)
	{
		while (size > 0)
		{
			--size;

			if (!IsNearEqual(*(startA + size), *(startB + size)))
			{
				return false;
			}
		}

		return true;
	}

	inline bool IsNearZero(float value)
	{
		return IsNearEqual(0, value);
	}

	template<typename TVector>
	inline TVector Project(const TVector& u, const TVector& v)
	{
		return v * (u.Dot(v) / v.Dot(v));
	}

	template<typename TVector>
	inline TVector Orthogonalize(const TVector& u, const TVector& v)
	{
		return (u - Project(u, v)).Normalize();
	}

	template<typename TVector>
	bool IsOrthogonalized(const TVector* start, const TVector* end)
	{
		if (start == end)
		{
			return true;
		}

		for (const TVector* current = start; current < end; ++current)
		{
			if (!current->IsNormalized())
			{
				return false;
			}

			for (const TVector* other = start; other < end; ++other)
			{
				if (current == other)
				{
					continue;
				}

				if (!current->IsOrthoginal(*other))
				{
					return false;
				}
			}
		}

		return true;
	}

	template<typename TVector>
	inline bool IsNormalized(const TVector& vector)
	{
		return IsNearEqual(1, vector.Magnitude());
	}

	enum class Axis:char
	{
		x,
		y,
		z,
		count,
		i = x,
		j = y,
		k = z
	};

	inline float ToRadians(const float degrees)
	{
		return degrees * PIf / 180;
	}
}