#include <gtest/gtest.h>
#include "../Math/BLMathFloat.h"

namespace BoulderLeaf::Math 
{
	TEST(Vector2, Initialization)
	{
		Vector2 vector(0, 4);

		EXPECT_EQ(vector.x, 0);
		EXPECT_EQ(vector.y, 4);
		EXPECT_EQ(vector.x, vector[0]);
		EXPECT_EQ(vector.y, vector[1]);
		EXPECT_EQ(vector.data[0], vector[0]);
		EXPECT_EQ(vector.data[1], vector[1]);
	}

	TEST(Vector2, Equality)
	{
		Vector2 vector(0, 4);

		EXPECT_EQ(vector, Vector2(0, 4));
		EXPECT_NE(vector, Vector2(0, 6));

		EXPECT_NE(vector, Vector2(-10.6f, 4.5f));
		EXPECT_NE(vector, Vector2(0, 4));
	}

	TEST(Vector2, Multiply)
	{
		Vector2 vector(10, 5.5f);
		Vector2 result = vector * Vector2(2, 3);
		EXPECT_EQ(result, Vector2(20, 16.5f));
		EXPECT_EQ(vector * 3.5f, Vector2(35, 19.25));

		vector = Vector2(10, 5.5f);
		vector *= Vector2(2, 2);
		EXPECT_EQ(vector, Vector2(20, 11));

		vector = Vector2(10, 5.5f);
		vector *= 3.0f;
		EXPECT_EQ(vector, Vector2(30, 16.5));
	}

	TEST(Vector2, Devide)
	{
		Vector2 vector(20, 30);
		Vector2 result = vector / Vector2(2, 3);
		EXPECT_EQ(result, Vector2(10, 10));
		EXPECT_EQ(vector / 2.0f, Vector2(10, 15));
		EXPECT_EQ(vector / Vector2(2, 2), Vector2(10, 15));

		vector = Vector2(20, 30);
		vector /= 2.0f;
		EXPECT_EQ(vector, Vector2(10, 15));

		vector = Vector2(20, 30);
		vector /= Vector2(2, 3);
		EXPECT_EQ(vector, Vector2(10, 10));
	}

	TEST(Vector2, Addition)
	{
		Vector2 vector(10, 5.5f);
		Vector2 result = vector + Vector2(2, 3);
		EXPECT_EQ(result, Vector2(12, 8.5f));

		vector = Vector2(10, 5.5f);
		vector += Vector2(5, 5);
		EXPECT_EQ(vector, Vector2(15, 10.5f));

		vector = Vector2(10, 5.5f);
		vector += 2.0f;
		EXPECT_EQ(vector, Vector2(12, 7.5f));
	}

	TEST(Vector2, Subtract)
	{
		Vector2 vector(10, 5.5f);
		Vector2 result = vector - Vector2(2, 3);
		EXPECT_EQ(result, Vector2(8, 2.5f));

		vector = Vector2(10, 5.5f);
		vector -= Vector2(5, 5);
		EXPECT_EQ(vector, Vector2(5, 0.5f));
	}

	TEST(Vector2, Cross)
	{
		Vector2 vector(1, 2);
		float result = vector.Cross(Vector2(1, 5));
		EXPECT_EQ(result, 3);
	}

	TEST(Vector2, Normalize)
	{
		Vector2 vector(1, 2);
		Vector2 result = vector.Normalize();
		EXPECT_EQ(result.Magnitude(), 1);
		EXPECT_GT(result.y, result.x);
	}

	TEST(Vector2, Magnitude)
	{
		Vector2 vector(2, 4);
		float result = vector.Magnitude();
		EXPECT_EQ(result, 6);
	}

	TEST(Vector2, Dot)
	{
		Vector2 vector(1, 2);
		float result = vector.Dot(Vector2(1, 5));
		EXPECT_EQ(result, 11);
	}
}