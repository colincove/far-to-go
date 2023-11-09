#include <gtest/gtest.h>
#include <blMatrix3x3.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(Matrix3x3, Multiplication_Vector3)
	{
		Matrix3x3 m(
			1, 3, 10,
			5, 4, 2,
			7, 8, 3
		);

		Vector3 v(1, 2, 3);
		Vector3 result = v * m;
		EXPECT_EQ(Vector3(32, 35, 23), result);
	}

	TEST(Matrix3x3, Transpose)
	{
		Matrix3x3 m(
			5, 10, 3,
			9, 2, 2,
			8, 9, 4
		);

		Matrix3x3 mt(m.Transpose());
		EXPECT_EQ(mt, Matrix3x3(
			5, 9, 8,
			10, 2, 9,
			3, 2, 4
		));
	}

	TEST(Matrix3x3, Identity)
	{
		Matrix3x3 m(
			5, 10, 3,
			9, 2, 2,
			8, 9, 4
		);

		Matrix3x3 result(m * Matrix3x3::Identity());
		EXPECT_EQ(m, result);
	}

	TEST(Matrix3x3, Minor)
	{
		Matrix3x3 m(
			9, 2, 2,
			8, 9, 4,
			1, 1, 2
		);

		EXPECT_EQ(m.Minor(0, 0), Matrix2x2(
			9, 4,
			1, 2
		));

		EXPECT_EQ(m.Minor(0, 1), Matrix2x2(
			8, 4,
			1, 2
		));

		EXPECT_EQ(m.Minor(0, 2), Matrix2x2(
			8, 9,
			1, 1
		));
	}
	TEST(Matrix3x3, MinorDeterminates)
	{
		Matrix3x3 m(
			4, 10, 5,
			7, 8, 2,
			3, 5, 5
		);

		Matrix2x2 m00(m.Minor(0, 0));
		Matrix2x2 m01(m.Minor(0, 1));
		Matrix2x2 m02(m.Minor(0, 2));

		EXPECT_EQ(m00, Matrix2x2(
			8, 2,
			5, 5
		));

		EXPECT_EQ(m01, Matrix2x2(
			7, 2,
			3, 5
		));

		EXPECT_EQ(m02, Matrix2x2(
			7, 8,
			3, 5
		));

		EXPECT_FLOAT_EQ(30, m00.Determinant());
		EXPECT_FLOAT_EQ(29, m01.Determinant());
		EXPECT_FLOAT_EQ(11, m02.Determinant());
	}

	TEST(Matrix3x3, Determinant)
	{
		Matrix3x3 m(
			4, 10, 5,
			7, 8, 2,
			3, 5, 5
		);

		float determinant(m.Determinant());
		EXPECT_FLOAT_EQ(-115, determinant);

		EXPECT_FLOAT_EQ(-8.0f, Matrix3x3(
			5, 0, 2,
			1, 2, -3,
			8, 2, -1
		).Determinant());
	}

	TEST(Matrix3x3, Adjoint)
	{
		Matrix3x3 m(
			1, 2, 3,
			4, 5, 6,
			7, 8, 9
		);

		Matrix3x3 adjoint(m.Adjoint());

		EXPECT_EQ(Matrix3x3(
			-3, 6, -3,
			6, -12, 6,
			-3, 6, -3
		), adjoint);

		Matrix3x3 m2(
			5, 0, 2,
			1, 2, -3,
			8, 2, -1
		);

		Matrix3x3 adjoint2(m2.Adjoint());
		Matrix3x3 adjoint2Expected(
			4, 4, -4,
			-23, -21, 17,
			-14, -10, 10);

		EXPECT_EQ(adjoint2, adjoint2Expected);
	}

	TEST(Matrix3x3, IsInvertible)
	{
		EXPECT_TRUE(Matrix3x3::k_isInvertible);
	}

	TEST(Matrix3x3, Inverse)
	{
		Matrix3x3 m(
			5, 0, 2,
			1, 2, -3,
			8, 2, -1
		);

		EXPECT_EQ(m.Inverse(), Matrix3x3(
			-(1.0f/2.0f), -(1.0f /2.0f), 1.0f /2.0f,
			23.0f /8.0f, 21.0f /8.0f, -(17.0f /8.0f),
			7.0f /4.0f, 5.0f /4.0f, -(5.0f /4.0f)
		));
	}
}