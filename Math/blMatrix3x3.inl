#pragma once

#include "..\Test\blAssert.h";

#include "blVector3.inl";

namespace BoulderLeaf::Math
{
	struct Matrix3x3
	{
		static constexpr size_t k_NumberOfRows = 3;
		static constexpr size_t k_NumberOfColumns = 3;
		static constexpr size_t k_NumberOfElements = k_NumberOfRows * k_NumberOfColumns;

		#define ELEMENT_INDEX(row, column) row * k_NumberOfColumns + column

		typedef Vector3 RowVector;
		typedef Vector3 ColumnVector;

		union
		{
			float elements[k_NumberOfElements];

			struct
			{
				float m00;
				float m01;
				float m02;
				float m10;
				float m11;
				float m12;
				float m20;
				float m21;
				float m22;
			};
		};

		Matrix3x3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
			: m00(m00), m01(m01), m02(m02),
			m10(m10), m11(m11), m12(m12),
			m20(m20), m21(m21), m22(m22)
		{

		}

		Matrix3x3() : Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0)
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
				GetElement(i, 1),
				GetElement(i, 2)
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

		inline Matrix3x3 Transpose() const
		{
			return Matrix3x3(
				m00, m10, m20,
				m01, m11, m21,
				m02, m12, m22
			);
		}

		static inline Matrix3x3 Identity()
		{
			return Matrix3x3(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			);
		}
	};

	inline bool operator==(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		return std::equal(std::begin(lhs.elements), std::end(lhs.elements), std::begin(rhs.elements));
	}

	inline bool operator!=(const Matrix3x3& lhs, const Matrix3x3& rhs) { return !(lhs == rhs); }

	inline Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		return Matrix3x3(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m02 + rhs.m02
			, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m12 + rhs.m12
			, lhs.m20 + rhs.m20, lhs.m21 + rhs.m21, lhs.m22 + rhs.m22);
	}

	inline Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		return Matrix3x3(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m02 - rhs.m02
			, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m12 - rhs.m12
			, lhs.m20 - rhs.m20, lhs.m21 - rhs.m21, lhs.m22 - rhs.m22);
	}

	inline Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		return Matrix3x3(lhs.GetRow(0).Dot(rhs.GetColumn(0)), lhs.GetRow(0).Dot(rhs.GetColumn(1)), lhs.GetRow(0).Dot(rhs.GetColumn(2)),
			lhs.GetRow(1).Dot(rhs.GetColumn(0)), lhs.GetRow(1).Dot(rhs.GetColumn(1)), lhs.GetRow(1).Dot(rhs.GetColumn(2)),
			lhs.GetRow(2).Dot(rhs.GetColumn(0)), lhs.GetRow(2).Dot(rhs.GetColumn(1)), lhs.GetRow(2).Dot(rhs.GetColumn(2)));
	}

	inline Matrix3x3 operator*(const Matrix3x3& lhs, const float& rhs)
	{
		return Matrix3x3(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs
			, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs
			, lhs.m20 * rhs, lhs.m21 * rhs, lhs.m22 * rhs);
	}

	inline void operator*=(Matrix3x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x3::k_NumberOfElements; i++)
		{
			lhs[i] *= rhs;
		}
	}

	inline Matrix3x3 operator/(const Matrix3x3& lhs, const float& rhs)
	{
		return Matrix3x3(
			lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs,
			lhs[3] / rhs, lhs[4] / rhs, lhs[5] / rhs,
			lhs[6] / rhs, lhs[7] / rhs, lhs[8] / rhs
		);
	}

	inline void operator/=(Matrix3x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x3::k_NumberOfElements; i++)
		{
			lhs[i] /= rhs;
		}
	}

	inline Matrix3x3 operator+(const Matrix3x3& lhs, const float& rhs)
	{
		return Matrix3x3(
			lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs,
			lhs[3] + rhs, lhs[4] + rhs, lhs[5] + rhs,
			lhs[6] + rhs, lhs[7] + rhs, lhs[8] + rhs
		);
	}

	inline void operator+=(Matrix3x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x3::k_NumberOfElements; i++)
		{
			lhs[i] += rhs;
		}
	}

	inline Matrix3x3 operator-(const Matrix3x3& lhs, const float& rhs)
	{
		return Matrix3x3(
			lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs,
			lhs[3] - rhs, lhs[4] - rhs, lhs[5] - rhs,
			lhs[6] - rhs, lhs[7] - rhs, lhs[8] - rhs
		);
	}

	inline void operator-=(Matrix3x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix3x3::k_NumberOfElements; i++)
		{
			lhs[i] -= rhs;
		}
	}

	inline Vector3 operator*(const Vector3& lhs, const Matrix3x3& rhs)
	{
		return Vector3(lhs.Dot(rhs.GetColumn(0)), lhs.Dot(rhs.GetColumn(1)), lhs.Dot(rhs.GetColumn(2)));
	}
}