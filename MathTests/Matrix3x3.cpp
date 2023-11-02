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
}