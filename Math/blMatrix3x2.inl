#pragma once

#include "..\Test\blAssert.h";

#include "blVector3.inl";
#include "blVector2.inl";
#include "blMatrix2x2.inl";
#include "blMatrix2x3.inl";

namespace BoulderLeaf::Math
{
	struct Matrix3x2
	{
		static constexpr size_t k_NumberOfRows = 3;
		static constexpr size_t k_NumberOfColumns = 2;
		static constexpr size_t k_NumberOfElements = k_NumberOfRows * k_NumberOfColumns;

#define ELEMENT_INDEX(row, column) row * k_NumberOfColumns + column

		typedef Vector2 RowVector;
		typedef Vector3 ColumnVector;

		union
		{
			float elements[k_NumberOfElements];

			struct
			{
				float m00;
				float m01;
				float m10;
				float m11;
				float m20;
				float m21;
			};
		};

		Matrix3x2(float m00, float m01, 
			float m10, float m11,
			float m20, float m21)
			: m00(m00), m01(m01),
			m10(m10), m11(m11), 
			m20(m20), m21(m21)
		{

		}

		Matrix3x2() : Matrix3x2(0, 0, 0, 0, 0, 0)
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

		inline float SetElement(const int row, const int column, const float value)
		{
			const size_t index = ELEMENT_INDEX(row, column);
			BL_ASSERT_INDEX(index, k_NumberOfElements);
			elements[index] = value;
		}

		inline float GetElement(const int row, const int column) const
		{
			const size_t index = ELEMENT_INDEX(row, column);
			BL_ASSERT_INDEX(index, k_NumberOfElements);
			return elements[index];
		}

		inline float& GetElementMutable(const int row, const int column)
		{
			const size_t index = ELEMENT_INDEX(row, column);
			BL_ASSERT_INDEX(index, k_NumberOfElements);
			return elements[index];
		}

		inline RowVector GetRow(const int i) const
		{
			BL_ASSERT_INDEX(i, k_NumberOfRows);
			return RowVector(
				GetElement(i, 0),
				GetElement(i, 1)
			);
		}

		inline ColumnVector GetColumn(const int j) const
		{
			BL_ASSERT_INDEX(j, k_NumberOfColumns);
			return ColumnVector(
				GetElement(0, j),
				GetElement(1, j),
				GetElement(2, j)
			);
		}

		static inline Matrix2x2 Identity()
		{
			return Matrix2x2(
				1, 0,
				0, 1
			);
		}

		inline Matrix2x3 Transpose() const
		{
			return Matrix2x3(
				m00, m10, m20, 
				m01, m11, m21
			);
		}
	};

	inline bool operator==(const Matrix3x2& lhs, const Matrix3x2& rhs)
	{
		return std::equal(std::begin(lhs.elements), std::end(lhs.elements), std::begin(rhs.elements));
	}

	inline bool operator!=(const Matrix3x2& lhs, const Matrix3x2& rhs) { return !(lhs == rhs); }

	inline Matrix3x2 operator+(const Matrix3x2& lhs, const Matrix3x2& rhs)
	{
		return Matrix3x2(
			lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, 
			lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, 
			lhs.m20 + rhs.m20, lhs.m21 + rhs.m21);
	}

	inline Matrix3x2 operator-(const Matrix3x2& lhs, const Matrix3x2& rhs)
	{
		return Matrix3x2(
			lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, 
			lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, 
			lhs.m20 - rhs.m20, lhs.m21 - rhs.m21);
	}

	inline Matrix3x2 operator*(const Matrix3x2& lhs, const float& rhs)
	{
		return Matrix3x2(
			lhs[0] * rhs, lhs[1] * rhs,
			lhs[2] * rhs, lhs[3] * rhs,
			lhs[4] * rhs, lhs[5] * rhs);
	}

	inline Matrix3x2 operator/(const Matrix3x2& lhs, const float& rhs)
	{
		return Matrix3x2(
			lhs[0] / rhs, lhs[1] / rhs,
			lhs[2] / rhs, lhs[3] / rhs,
			lhs[4] / rhs, lhs[5] / rhs);
	}

	inline void operator*=(Matrix3x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] *= rhs;
		}
	}

	inline void operator/=(Matrix3x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] /= rhs;
		}
	}

	inline Matrix3x2 operator+(const Matrix3x2& lhs, const float& rhs)
	{
		return Matrix3x2(
			lhs[0] + rhs, lhs[1] + rhs, 
			lhs[2] + rhs, lhs[3] + rhs,
			lhs[4] + rhs, lhs[5] + rhs);
	}

	inline void operator+=(Matrix3x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] += rhs;
		}
	}

	inline Matrix3x2 operator-(const Matrix3x2& lhs, const float& rhs)
	{
		return Matrix3x2(
			lhs[0] - rhs, lhs[1] - rhs,
			lhs[2] - rhs, lhs[3] - rhs, 
			lhs[4] - rhs, lhs[5] - rhs);
	}

	inline void operator-=(Matrix3x2& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x2::k_NumberOfElements; i++)
		{
			lhs.elements[i] -= rhs;
		}
	}
}