#pragma once

#include "blVector3.inl";
#include "blVector2.inl";
#include "blMatrix3x3.inl";

namespace BoulderLeaf::Math
{
	struct Matrix2x3
	{
#define ELEMENT_INDEX(i, j) i * 3 + j

		static constexpr size_t s_NumberOfElements = 6;

		typedef Vector3 RowVector;
		typedef Vector2 ColumnVector;

		union
		{
			float elements[s_NumberOfElements];

			struct
			{
				float m00;
				float m01;
				float m02;
				float m10;
				float m11;
				float m12;
			};
		};

		Matrix2x3(float m00, float m01, float m02,
			float m10, float m11, float m12)
			: m00(m00), m01(m01), m02(m02),
			m10(m10), m11(m11), m12(m12)
		{

		}

		Matrix2x3() : Matrix2x3(0, 0, 0, 0, 0, 0)
		{

		}

		const float& operator[](const size_t i) const
		{
			return elements[i];
		}

		float& operator[](const size_t i)
		{
			return elements[i];
		}

		inline float SetElement(const int row, const int column, const float value)
		{
			elements[ELEMENT_INDEX(row, column)] = value;
		}

		inline float GetElement(const int row, const int column) const
		{
			return elements[ELEMENT_INDEX(row, column)];
		}

		inline float& GetElementMutable(const int row, const int column)
		{
			return elements[ELEMENT_INDEX(row, column)];
		}

		inline RowVector GetRow(const int i) const
		{
			return RowVector(
				GetElement(i, 0),
				GetElement(i, 1),
				GetElement(i, 2)
			);
		}

		inline ColumnVector GetColumn(const int j) const
		{
			return ColumnVector(
				GetElement(0, j),
				GetElement(1, j)
			);
		}
	};

	inline bool operator==(const Matrix2x3& lhs, const Matrix2x3& rhs)
	{
		return std::equal(std::begin(lhs.elements), std::end(lhs.elements), std::begin(rhs.elements));
	}

	inline bool operator!=(const Matrix2x3& lhs, const Matrix2x3& rhs) { return !(lhs == rhs); }

	inline Matrix2x3 operator+(const Matrix2x3& lhs, const Matrix2x3& rhs)
	{
		return Matrix2x3(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m02 + rhs.m02
			, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m12 + rhs.m12);
	}

	inline Matrix2x3 operator-(const Matrix2x3& lhs, const Matrix2x3& rhs)
	{
		return Matrix2x3(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m02 - rhs.m02
			, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m12 - rhs.m12);
	}

	inline Matrix2x3 operator*(const Matrix2x3& lhs, const Matrix2x3& rhs)
	{
		return Matrix2x3(lhs.m00 * rhs.m00, lhs.m01 * rhs.m01, lhs.m02 * rhs.m02
			, lhs.m10 * rhs.m10, lhs.m11 * rhs.m11, lhs.m12 * rhs.m12);
	}

	inline Matrix2x3 operator/(const Matrix2x3& lhs, const Matrix2x3& rhs)
	{
		return Matrix2x3(lhs.m00 / rhs.m00, lhs.m01 / rhs.m01, lhs.m02 / rhs.m02
			, lhs.m10 / rhs.m10, lhs.m11 / rhs.m11, lhs.m12 / rhs.m12);
	}

	inline Matrix2x3 operator*(const Matrix2x3& lhs, const float& rhs)
	{
		return Matrix2x3(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs
			, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs);
	}

	inline Matrix2x3 operator/(const Matrix2x3& lhs, const float& rhs)
	{
		return Matrix2x3(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m02 / rhs
			, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m12 / rhs);
	}

	inline void operator*=(Matrix2x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x3::s_NumberOfElements; i++)
		{
			lhs.elements[i] *= rhs;
		}
	}

	inline void operator/=(Matrix2x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x3::s_NumberOfElements; i++)
		{
			lhs.elements[i] /= rhs;
		}
	}

	inline Matrix2x3 operator+(const Matrix2x3& lhs, const float& rhs)
	{
		return Matrix2x3(lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs,
			lhs[3] + rhs, lhs[4] + rhs, lhs[5] + rhs);
	}

	inline void operator+=(Matrix2x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x3::s_NumberOfElements; i++)
		{
			lhs.elements[i] += rhs;
		}
	}

	inline Matrix2x3 operator-(const Matrix2x3& lhs, const float& rhs)
	{
		return Matrix2x3(lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs,
			lhs[3] - rhs, lhs[4] - rhs, lhs[5] - rhs);
	}

	inline void operator-=(Matrix2x3& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix2x3::s_NumberOfElements; i++)
		{
			lhs.elements[i] -= rhs;
		}
	}

	inline Matrix2x3 operator*(const Matrix2x3& lhs, const Matrix3x3 rhs)
	{
		return Matrix2x3(
		lhs.GetRow(0).Dot(rhs.GetColumn(0)), lhs.GetRow(0).Dot(rhs.GetColumn(1)), lhs.GetRow(0).Dot(rhs.GetColumn(2)),
		lhs.GetRow(1).Dot(rhs.GetColumn(0)), lhs.GetRow(1).Dot(rhs.GetColumn(1)), lhs.GetRow(1).Dot(rhs.GetColumn(2))
		);
	}
}