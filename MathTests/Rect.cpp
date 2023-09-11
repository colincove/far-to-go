#include <gtest/gtest.h>
#include <blRect.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(Rect, Initialization)
	{
		Rect rect(5, 10, Vector2(20, 30));

		EXPECT_EQ(rect.halfWidth, 5);
		EXPECT_EQ(rect.halfHeight, 10);
		EXPECT_EQ(rect.center, Vector2(20, 30));
	}

	TEST(Rect, Bounds)
	{
		Rect rect(5, 10, Vector2(20, 30));

		EXPECT_FLOAT_EQ(rect.Left(), 15);
		EXPECT_FLOAT_EQ(rect.Right(), 25);
		EXPECT_FLOAT_EQ(rect.Top(), 40);
		EXPECT_FLOAT_EQ(rect.Bottom(), 20);
	}

	TEST(Rect, Corners)
	{
		Rect rect(5, 10, Vector2(20, 30));

		EXPECT_EQ(rect.TopLeft(), Vector2(15, 40));
		EXPECT_EQ(rect.TopRight(), Vector2(25, 40));
		EXPECT_EQ(rect.BottomLeft(), Vector2(15, 20));
		EXPECT_EQ(rect.BottomRight(), Vector2(25, 20));
	}

	TEST(Rect, Overlap)
	{
		Rect rect(5, 10, Vector2(20, 30));
		Rect result = rect.Overlap(Rect(10, 20, Vector2(10, 55)));
		Rect expected(2.5f, 2.5f, Vector2(17.5f, 37.5f));
		EXPECT_EQ(result, expected);
	}

	TEST(Rect, Area)
	{
		Rect rect(5, 10, Vector2(20, 30));
		EXPECT_FLOAT_EQ(rect.Area(), 200);
	}
}