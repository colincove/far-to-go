#include <gtest/gtest.h>
#include <blMath.inl>
#include <array>

namespace BoulderLeaf::Math::Test
{
	TEST(Math, NearEqual)
	{
		constexpr static size_t size = 3;

		float a[]{ 1.66f, 7.77f, 4.44f };
		float b[]{ 1.66f, 7.77f, 4.44f };
		float c[]{ 1.66f, 7.77f, 9.99f };
		float d[]{ 1.66f, 8.77f, 9.99f };
		float e[]{ 8.88f, 7.77f, 4.44f };

		EXPECT_TRUE(IsNearEqual(std::begin(a), std::begin(b), size));
		EXPECT_FALSE(IsNearEqual(std::begin(a), std::begin(c), size));
		EXPECT_FALSE(IsNearEqual(std::begin(a), std::begin(d), size));
		EXPECT_FALSE(IsNearEqual(std::begin(a), std::begin(e), size));
	}
}