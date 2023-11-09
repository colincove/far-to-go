#include <gtest/gtest.h>
#include <blMatrix2x2.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(Matrix2x2, Determinant)
	{
		Matrix2x2 m(
			2, 3,
			4, 5
		);

		float determinant(m.Determinant());
		EXPECT_FLOAT_EQ(-2.0f, determinant);
	}

	TEST(Matrix2x2, IsInvertible)
	{
		EXPECT_TRUE(Matrix2x2::k_isInvertible);
	}

	TEST(Matrix2x2, HasValidInverse)
	{
		EXPECT_TRUE(Matrix2x2(2, 5, 7, 2).HasValidInverse());
		EXPECT_FALSE(Matrix2x2().HasValidInverse());
	}

	TEST(Matrix2x2, Inverse)
	{
		Matrix2x2 m(
			3, 0,
			-1, 2
		);

		EXPECT_EQ(m.Inverse(), Matrix2x2(
			1.0f / 3.0f, 0,
			-(1.0f / 6.0f), 1.0f / 2.0f
		));
	}
}