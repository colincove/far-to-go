#pragma once

namespace BoulderLeaf::Math
{
	static float Epsilon = 0.000001f;

	inline bool IsNearExpected(float expected, float actual)
	{
		return expected - Epsilon < actual&& expected + Epsilon > actual;
	}

	inline bool IsNearZero(float value)
	{
		return IsNearExpected(0, value);
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
	static bool IsOrthogonalized(const TVector* start, const TVector* end)
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
		return IsNearExpected(1, vector.Magnitude());
	}
}