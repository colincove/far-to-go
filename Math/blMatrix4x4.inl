#pragma once

#include "blVector4.inl";

namespace BoulderLeaf::Math
{
	struct Matrix4x4
	{
		#define ELEMENT_INDEX(i, j) i * 4 + j

		static constexpr size_t s_NumberOfElements = 16;

		typedef Vector4 RowVector;
		typedef Vector4 ColumnVector;

		union
		{
			float elements[s_NumberOfElements];

			struct
			{
				float m00;
				float m01;
				float m02;
				float m03;
				float m10;
				float m11;
				float m12;
				float m13;
				float m20;
				float m21;
				float m22;
				float m23;
				float m30;
				float m31;
				float m32;
				float m33;
			};
		};

		Matrix4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: m00(m00), m01(m01), m02(m02), m03(m03),
			m10(m10), m11(m11), m12(m12), m13(m13),
			m20(m20), m21(m21), m22(m22), m23(m23),
			m30(m30), m31(m31), m32(m32), m33(m33)
		{

		}

		Matrix4x4() : Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
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
				GetElement(i, 2),
				GetElement(i, 3)
			);
		}

		inline ColumnVector GetColumn(const int j) const
		{
			return ColumnVector(
				GetElement(0, j),
				GetElement(1, j),
				GetElement(2, j),
				GetElement(3, j)
			);
		}

		inline Matrix4x4 Transpose() const
		{
			return Matrix4x4(
				m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33
			);
		}

		static inline Matrix4x4 Identity()
		{
			return Matrix4x4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
		}
	};

	inline bool operator==(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		return std::equal(std::begin(lhs.elements), std::end(lhs.elements), std::begin(rhs.elements));
	}

	inline bool operator!=(const Matrix4x4& lhs, const Matrix4x4& rhs) { return !(lhs == rhs); }

	inline Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		return Matrix4x4(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m02 + rhs.m02, lhs.m03 + rhs.m03
		, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13
		, lhs.m20 + rhs.m20, lhs.m21 + rhs.m21, lhs.m22 + rhs.m22, lhs.m23 + rhs.m23
		, lhs.m30 + rhs.m30, lhs.m31 + rhs.m31, lhs.m32 + rhs.m32, lhs.m33 + rhs.m33);
	}

	inline Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		return Matrix4x4(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m02 - rhs.m02, lhs.m03 - rhs.m03
			, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13
			, lhs.m20 - rhs.m20, lhs.m21 - rhs.m21, lhs.m22 - rhs.m22, lhs.m23 - rhs.m23
			, lhs.m30 - rhs.m30, lhs.m31 - rhs.m31, lhs.m32 - rhs.m32, lhs.m33 - rhs.m33);
	}

	inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
	{
		return Matrix4x4(lhs.GetRow(0).Dot(rhs.GetColumn(0)), lhs.GetRow(0).Dot(rhs.GetColumn(1)), lhs.GetRow(0).Dot(rhs.GetColumn(2)), lhs.GetRow(0).Dot(rhs.GetColumn(3)),
			lhs.GetRow(1).Dot(rhs.GetColumn(0)), lhs.GetRow(1).Dot(rhs.GetColumn(1)), lhs.GetRow(1).Dot(rhs.GetColumn(2)), lhs.GetRow(1).Dot(rhs.GetColumn(3)),
			lhs.GetRow(2).Dot(rhs.GetColumn(0)), lhs.GetRow(2).Dot(rhs.GetColumn(1)), lhs.GetRow(2).Dot(rhs.GetColumn(2)), lhs.GetRow(2).Dot(rhs.GetColumn(3)),
			lhs.GetRow(3).Dot(rhs.GetColumn(0)), lhs.GetRow(3).Dot(rhs.GetColumn(1)), lhs.GetRow(3).Dot(rhs.GetColumn(2)), lhs.GetRow(3).Dot(rhs.GetColumn(3)));
	}

	inline Matrix4x4 operator*(const Matrix4x4& lhs, const float& rhs)
	{
		return Matrix4x4(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs, lhs.m03 * rhs
			, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs, lhs.m13 * rhs
			, lhs.m20 * rhs, lhs.m21 * rhs, lhs.m22 * rhs, lhs.m23 * rhs
			, lhs.m30 * rhs, lhs.m31 * rhs, lhs.m32 * rhs, lhs.m33 * rhs);
	}

	inline Matrix4x4 operator/(const Matrix4x4& lhs, const float& rhs)
	{
		return Matrix4x4(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m02 / rhs, lhs.m03 / rhs
			, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m12 / rhs, lhs.m13 / rhs
			, lhs.m20 / rhs, lhs.m21 / rhs, lhs.m22 / rhs, lhs.m23 / rhs
			, lhs.m30 / rhs, lhs.m31 / rhs, lhs.m32 / rhs, lhs.m33 / rhs);
	}

	inline void operator*=(Matrix4x4& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix4x4::s_NumberOfElements; i++)
		{
			lhs.elements[i] *= rhs;
		}
	}

	inline void operator/=(Matrix4x4& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix4x4::s_NumberOfElements; i++)
		{
			lhs.elements[i] /= rhs;
		}
	}

	inline Matrix4x4 operator+(const Matrix4x4& lhs, const float& rhs)
	{
		return Matrix4x4(lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs, lhs[3] + rhs,
			lhs[4] + rhs, lhs[5] + rhs, lhs[6] + rhs, lhs[7] + rhs,
			lhs[8] + rhs, lhs[9] + rhs, lhs[10] + rhs, lhs[11] + rhs,
			lhs[12] + rhs, lhs[13] + rhs, lhs[14] + rhs, lhs[15] + rhs);
	}

	inline void operator+=(Matrix4x4& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix4x4::s_NumberOfElements; i++)
		{
			lhs.elements[i] += rhs;
		}
	}

	inline Matrix4x4 operator-(const Matrix4x4& lhs, const float& rhs)
	{
		return Matrix4x4(lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs, lhs[3] -  rhs,
			lhs[4] - rhs, lhs[5] - rhs, lhs[6] - rhs, lhs[7] - rhs,
			lhs[8] - rhs, lhs[9] - rhs, lhs[10] - rhs, lhs[11] - rhs,
			lhs[12] - rhs, lhs[13] - rhs, lhs[14] - rhs, lhs[15] - rhs);
	}

	inline void operator-=(Matrix4x4& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix4x4::s_NumberOfElements; i++)
		{
			lhs.elements[i] -= rhs;
		}
	}
}