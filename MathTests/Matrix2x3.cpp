#include <gtest/gtest.h>
#include <blMatrix2x3.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(blMatrix2x3, Multiplication_3x3)
	{
		Matrix2x3 m2x3(
			-1, 5, -4, 
			3, 2, 1);
		Matrix3x3 m3x3(
			2, 1, 0,
			0, -2, 1,
			-1, 2, 3
		);

		Matrix2x3 result = m2x3 * m3x3;

		EXPECT_EQ(
			Matrix2x3(
				2, -19, -7,
				5, 1, 5),
			result
		);
	}
}