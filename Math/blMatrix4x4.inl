#pragma once

#include "..\Test\blAssert.h"

#include <blVector4.inl>
#include <blMatrix3x3.inl>

#include <math.h>

namespace BoulderLeaf::Math
{
	//need to forward declare these in order for the Inverse function to link
	struct Matrix4x4;
	inline Matrix4x4 operator/(const Matrix4x4& lhs, const float& rhs);
	inline Vector4 operator*(const Vector4& lhs, const Matrix4x4& rhs);

	struct Matrix4x4
	{
		static constexpr size_t k_NumberOfRows = 4;
		static constexpr size_t k_NumberOfColumns = 4;
		static constexpr size_t k_NumberOfElements = k_NumberOfRows * k_NumberOfColumns;

		static constexpr bool k_isInvertible = k_NumberOfColumns == k_NumberOfRows;

		#define ELEMENT_INDEX(row, column) row * k_NumberOfColumns + column

		typedef Vector4 RowVector;
		typedef Vector4 ColumnVector;

		union
		{
			float elements[k_NumberOfElements];

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

			struct
			{
				RowVector r0;
				RowVector r1;
				RowVector r2;
				RowVector r3;
			};
		};

		constexpr Matrix4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: m00(m00), m01(m01), m02(m02), m03(m03),
			m10(m10), m11(m11), m12(m12), m13(m13),
			m20(m20), m21(m21), m22(m22), m23(m23),
			m30(m30), m31(m31), m32(m32), m33(m33)
		{

		}

		constexpr Matrix4x4(RowVector r0, RowVector r1, RowVector r2, RowVector r3)
			: r0(r0), r1(r1), r2(r2), r3(r3)
		{

		}

		constexpr Matrix4x4() : Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
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

		inline float SetElement(const size_t row, const size_t column, const float value)
		{
			elements[ELEMENT_INDEX(row, column)] = value;
		}

		inline float GetElement(const size_t row, const size_t column) const
		{
			return elements[ELEMENT_INDEX(row, column)];
		}

		inline float& GetElementMutable(const size_t row, const size_t column)
		{
			return elements[ELEMENT_INDEX(row, column)];
		}

		inline RowVector GetRow(const size_t i) const
		{
			return RowVector(
				GetElement(i, 0),
				GetElement(i, 1),
				GetElement(i, 2),
				GetElement(i, 3)
			);
		}

		inline ColumnVector GetColumn(const size_t j) const
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

		Matrix3x3 Minor(const size_t row, const size_t column) const
		{
			BL_ASSERT_INDEX(row, k_NumberOfRows);
			BL_ASSERT_INDEX(column, k_NumberOfColumns);

			Matrix3x3 result = Matrix3x3(0, 0, 0, 0, 0,0, 0, 0, 0);

			for (size_t i = 0; i < Matrix3x3::k_NumberOfRows; ++i)
			{
				for (size_t j = 0; j < Matrix3x3::k_NumberOfColumns; ++j)
				{
					float& element = result.GetElementMutable(i, j);
					element = GetElement(i >= row ? i + 1 : i, j >= column ? j + 1 : j);
				}
			}

			return result;
		}

		inline float Determinant() const
		{
			return m00 * Minor(0,0).Determinant() - 
				m01 * Minor(0, 1).Determinant() + 
				m02 * Minor(0, 2).Determinant() - 
				m03 * Minor(0, 3).Determinant();
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

		inline Matrix4x4 Adjoint() const
		{
			return Cofactor().Transpose();
		}

		inline Matrix4x4 Cofactor() const
		{
			return Matrix4x4(
				Cofactor(0, 0), Cofactor(0, 1), Cofactor(0, 2), Cofactor(0, 3),
				Cofactor(1, 0), Cofactor(1, 1), Cofactor(1, 2), Cofactor(1, 3),
				Cofactor(2, 0), Cofactor(2, 1), Cofactor(2, 2), Cofactor(2, 3),
				Cofactor(3, 0), Cofactor(3, 1), Cofactor(3, 2), Cofactor(3, 3)
			);
		}

		inline float Cofactor(const size_t row, const size_t column) const
		{
			BL_ASSERT_INDEX(row, k_NumberOfRows);
			BL_ASSERT_INDEX(column, k_NumberOfColumns);

			return powf(-1, static_cast<float>(row + column + 2)) * Minor(row, column).Determinant();
		}

		inline bool HasValidInverse() const
		{
			return !IsNearZero(Determinant());
		}

		inline Matrix4x4 Inverse() const
		{
			return Adjoint() / Determinant();
		}

		static inline Matrix4x4 ScalingMatrix(const Vector3& scale)
		{
			return ScalingMatrix(scale.x, scale.y, scale.z);
		}

		static inline Matrix4x4 ScalingMatrix(const float& x, const float& y, const float& z)
		{
			return Matrix4x4(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			);
		}

		static inline Matrix4x4 Transform(const Vector3& translation, const Vector3& scale, const Vector3& rotationAxis, const float r)
		{
			const float c(cosf(r));
			const float s(sinf(r));

			return Matrix4x4(
				c + (1 - c) * (rotationAxis.x * rotationAxis.x), (1 - c) * rotationAxis.x * rotationAxis.y + s * rotationAxis.z, (1 - c) * (rotationAxis.x * rotationAxis.z) - (s * rotationAxis.y), 0,
				(1 - c) * (rotationAxis.x * rotationAxis.y) - (s * rotationAxis.z), c + (1 - c) * (rotationAxis.y * rotationAxis.y), (1 - c) * (rotationAxis.y * rotationAxis.z) + (s * rotationAxis.x), 0,
				(1 - c) * (rotationAxis.x * rotationAxis.z) + (s * rotationAxis.y), (1 - c) * (rotationAxis.y * rotationAxis.z) - (s * rotationAxis.x), c + (1 - c) * (rotationAxis.z * rotationAxis.z), 0,
				translation.x, translation.y, translation.z, 1
			);
		}

		static inline Matrix4x4 RotationMatrix(const Vector3& axis, const float r)
		{
			return RotationMatrix(axis.x, axis.y, axis.z, r);
		}

		static inline Matrix4x4 RotationMatrix(const float& x, const float& y, const float& z, const float r)
		{
			const float c(cosf(r));
			const float s(sinf(r));

			return Matrix4x4(
				c + (1 - c) * (x * x), (1 - c) * x * y + s * z, (1 - c) * (x * z) - (s * y), 0,
				(1 - c) * (x * y) - (s * z), c + (1 - c) * (y * y), (1 - c) * (y * z) + (s * x), 0,
				(1 - c) * (x * z) + (s * y), (1 - c) * (y * z) - (s * x), c + (1 - c) * (z * z), 0,
				0, 0, 0, 1
			);
		}

		static inline Matrix4x4 TranslationMatrix(const Vector3& translation)
		{
			return TranslationMatrix(translation.x, translation.y, translation.z);
		}

		static inline Matrix4x4 TranslationMatrix(const float& x, const float& y, const float& z)
		{
			return Matrix4x4(
				1, 0, 0, 0,
				0, 1, 0, 0, 
				0, 0, 1, 0,
				x, y, z, 1
			);
		}

		static inline Vector4 Transform(const Matrix4x4& matrix, const Vector4& vector)
		{
			return vector * matrix;
		}

		static inline Matrix4x4 LookToRH(const Vector4& from, const Vector4& dir, const Vector4& arbitraryUp)
		{
			//TODO: Test. Verify. 
			return LookAtRH(from, from + dir, arbitraryUp);
		}

		static inline Matrix4x4 LookToLH(const Vector4& from, const Vector4& dir, const Vector4& arbitraryUp)
		{
			//TODO: Test. Verify. 
			return LookAtLH(from, from + dir, arbitraryUp);
		}

		static inline Matrix4x4 LookAtLH(const Vector4& from, const Vector4& to, const Vector4& arbitraryUp)
		{
            // Match DirectXMath XMMatrixLookAtRH for a right-handed, row-major view matrix.
			// zaxis = normalize(eye - focus) for RH; xaxis = normalize(cross(up, zaxis)); yaxis = cross(zaxis, xaxis)
			const Vector4 forward = (to - from).Normalize(); // forward from camera into scene
			const Vector4 right = arbitraryUp.Cross(forward).Normalize(); // right
			const Vector4 up = forward.Cross(right); // up

			return Matrix4x4(
				right.x, right.y, right.z, 0,
				up.x, up.y, up.z, 0,
				forward.x, forward.y, forward.z, 0,
				-right.Dot(from), -up.Dot(from), -forward.Dot(from), 1);
		}

		static inline Matrix4x4 LookAtRH(const Vector4& from, const Vector4& to, const Vector4& arbitraryUp)
		{
			// Match DirectXMath XMMatrixLookAtRH for a right-handed, row-major view matrix.
			// zaxis = normalize(eye - focus) for RH; xaxis = normalize(cross(up, zaxis)); yaxis = cross(zaxis, xaxis)
			const Vector4 forward = (from - to).Normalize(); // forward from camera into scene
			const Vector4 right = arbitraryUp.Cross(forward).Normalize() * -1; // right
			const Vector4 up = forward.Cross(right); // up

			return Matrix4x4(
				right.x, right.y, right.z, 0,
				up.x, up.y, up.z, 0,
				forward.x, forward.y, forward.z, 0,
				-right.Dot(from), -up.Dot(from), -forward.Dot(from), 1);
		}

		static inline Matrix4x4 ViewMatrix(const Vector4& from, const Vector4& to, const Vector4& arbitraryUp)
		{
			//https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function/framing-lookat-function.html
			//This method does not calculate the camera-to-world matrix, but rather the world-to-camera matrix—its inverse. Typically, this matrix is desired in programs that use real-time graphics APIs; it is often referred to as the view matrix. It is used in the vertex shader to transform vertices from world space to camera space. Therefore, using this method to directly obtain the world-to-camera matrix can be more convenient than the method provided above, which yields the camera-to-world matrix instead.
			//TLDR: LookAt makes logical sense and is easier to understand, but ViewMatrix is more directly useful for graphics programming. Both are valid and can be used based on preference and context.
			const Matrix4x4 lookAtMatrix = LookAtLH(from, to, arbitraryUp); // Camera to world
			const Matrix4x4 lookAtMatrixInverse = lookAtMatrix.Inverse();
			return Matrix4x4(lookAtMatrixInverse.r0, lookAtMatrixInverse.r1, lookAtMatrixInverse.r2, lookAtMatrix.r3); //world to camera
			 // Invert the LookAt matrix to get the view matrix (world to camera)
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
		for (int i = 0; i < Matrix4x4::k_NumberOfElements; i++)
		{
			lhs.elements[i] *= rhs;
		}
	}

	inline void operator/=(Matrix4x4& lhs, const float& rhs)
	{
		for (int i = 0; i < Matrix4x4::k_NumberOfElements; i++)
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
		for (int i = 0; i < Matrix4x4::k_NumberOfElements; i++)
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
		for (int i = 0; i < Matrix4x4::k_NumberOfElements; i++)
		{
			lhs.elements[i] -= rhs;
		}
	}

	inline Vector4 operator*(const Vector4& lhs, const Matrix4x4& rhs)
	{
		return Vector4(lhs.Dot(rhs.GetColumn(0)), lhs.Dot(rhs.GetColumn(1)), lhs.Dot(rhs.GetColumn(2)) , lhs.Dot(rhs.GetColumn(3)));
	}

	inline bool NearEqual(const Matrix4x4& a, const Matrix4x4& b)
	{
		for (int i = 0; i < Matrix4x4::k_NumberOfElements; ++i)
		{
			if (!IsNearEqual(a.elements[i], b.elements[i]))
			{
				return false;
			}
		}

		return true;
	}
}