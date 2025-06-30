#pragma once

#include "../Test/blAssert.h"

#include <blVector2.inl>

namespace BoulderLeaf::Math
{
	//need to forward declare these in order for the Inverse function to link
	struct Matrix2x2;
	inline Matrix2x2 operator*(const Matrix2x2& lhs, const float& rhs);

	struct Matrix2x2
	{
		static constexpr size_t k_NumberOfRows = 2;
		static constexpr size_t k_NumberOfColumns = 2;
		static constexpr size_t k_NumberOfElements = k_NumberOfRows * k_NumberOfColumns;

		static constexpr bool k_isInvertible = k_NumberOfColumns == k_NumberOfRows;

#define ELEMENT_INDEX(row, column) row * k_NumberOfColumns + column

		typedef Vector2 RowVector;
		typedef Vector2 ColumnVector;

		union
		{
			float elements[k_NumberOfElements];

			struct
			{
				float m00;
				float m01;
				float m10;
				float m11;
			};
		};

		constexpr Matrix2x2(float m00, float m01,
			float m10, float m11) : m00(m00), m01(m01), m10(m10), m11(m11)		
		{

		}

		constexpr Matrix2x2() : Matrix2x2(0, 0, 0, 0)
		{

		}

		const float& operator[](const size_t i) const
		{
			BL_ASSERT_INDEX(i, k_NumberOfElements);
			return elements[i];
		}

		float& operator[](const size_t i)
		{
			BL_ASSERT_INDEX(i, k_NumberOfElements);
			return elements[i];
		}

		inline float GetElement(const size_t row, const size_t column) const
		{
			const size_t index = ELEMENT_INDEX(row, column);
			BL_ASSERT_INDEX(index, k_NumberOfElements);
			return elements[index];
		}

		inline float& GetElementMutable(const size_t row, const size_t column)
		{
			const size_t index = ELEMENT_INDEX(row, column);
			BL_ASSERT_INDEX(index, k_NumberOfElements);
			return elements[index];
		}

		inline RowVector GetRow(const size_t i) const
		{
			BL_ASSERT_INDEX(i, k_NumberOfRows);
			return RowVector(
				GetElement(i, 0),
				GetElement(i, 1)
			);
		}

		inline ColumnVector GetColumn(const size_t j) const
		{
			BL_ASSERT_INDEX(j, k_NumberOfColumns);
			return ColumnVector(
				GetElement(0, j),
				GetElement(1, j)
			);
		}

		static inline Matrix2x2 Identity()
		{
			return Matrix2x2(
				1, 0,
				0, 1
			);
		}

		inline Matrix2x2 Transpose() const
		{
			return Matrix2x2(
				m00, m10,
				m01, m11
			);
		}

		inline float Determinant() const
		{
			return m00 * m11 - m01 * m10;
		}

		inline bool HasValidInverse()
		{
			return !IsNearZero(Determinant());
		}

		inline Matrix2x2 Inverse()
		{
			return Matrix2x2(
				m11, m01,
				m10, m00) * (1 / Determinant());
		}
	};

	inline bool operator==(const Matrix2x2& lhs, const Matrix2x2& rhs)
	{
		return std::equal(std::begin(lhs.elements), std::end(lhs.elements), std::begin(rhs.elements));
	}

	inline bool operator!=(const Matrix2x2& lhs, const Matrix2x2& rhs) { return !(lhs == rhs); }

	inline Matrix2x2 operator+(const Matrix2x2& lhs, const Matrix2x2& rhs)
	{
		return Matrix2x2(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11);
	}

	inline Matrix2x2 operator-(const Matrix2x2& lhs, const Matrix2x2& rhs)
	{
		return Matrix2x2(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11);
	}

	inline Matrix2x2 operator*(const Matrix2x2& lhs, const float& rhs)
	{
		return Matrix2x2(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m10 * rhs, lhs.m11 * rhs);
	}

	inline Matrix2x2 operator/(const Matrix2x2& lhs, const float& rhs)
	{
		return Matrix2x2(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m10 / rhs, lhs.m11 / rhs);
	}

	inline void operator*=(Matrix2x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] *= rhs;
		}
	}

	inline void operator/=(Matrix2x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] /= rhs;
		}
	}

	inline Matrix2x2 operator+(const Matrix2x2& lhs, const float& rhs)
	{
		return Matrix2x2(lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs, lhs[3]);
	}

	inline void operator+=(Matrix2x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] += rhs;
		}
	}

	inline Matrix2x2 operator-(const Matrix2x2& lhs, const float& rhs)
	{
		return Matrix2x2(lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs, lhs[3] - rhs);
	}

	inline void operator-=(Matrix2x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] -= rhs;
		}
	}

	inline Matrix2x2 operator*(const Matrix2x2& lhs, const Matrix2x2 rhs)
	{
		return Matrix2x2(
			lhs.GetRow(0).Dot(rhs.GetColumn(0)), lhs.GetRow(0).Dot(rhs.GetColumn(1)), 
			lhs.GetRow(1).Dot(rhs.GetColumn(0)), lhs.GetRow(1).Dot(rhs.GetColumn(1))
		);
	}
}