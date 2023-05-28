#pragma once

namespace BoulderLeaf::Math
{
	template<typename T>
	inline void Normalize(T& x, T& y);

	template<typename T>
	inline void Normalize(T& x, T& y, T& z);

	template<typename T>
	inline T Sum(T x, T y, T z);
}

#include "BLMath.inl"