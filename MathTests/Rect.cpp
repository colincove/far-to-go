#include <gtest/gtest.h>
#include "../Math/BLMathFloat.h"

namespace BoulderLeaf::Math
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

		EXPECT_EQ(rect.TopLeft(), Vector2(rect.Left(), rect.Top()));
		EXPECT_EQ(rect.TopRight(), Vector2(rect.Right(), rect.Top()));
		EXPECT_EQ(rect.BottomLeft(), Vector2(rect.Left(), rect.Bottom()));
		EXPECT_EQ(rect.BottomRight(), Vector2(rect.Left(), rect.Bottom()));
	}

	TEST(Rect, Overlap)
	{
		Rect rect(5, 10, Vector2(20, 30));
		Rect result = rect.Overlap(Rect(10, 20, Vector2(10, 55)));
		Rect expected(5, 5, Vector2(17.5, 35));
		EXPECT_EQ(result, expected);
	}

	TEST(Rect, Area)
	{
		Rect rect(5, 10, Vector2(20, 30));
		EXPECT_EQ(rect.Area(), 200);
	}
}