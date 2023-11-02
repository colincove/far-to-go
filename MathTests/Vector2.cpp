#include <gtest/gtest.h>
#include <blVector2.inl>

namespace BoulderLeaf::Math::Test
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
		EXPECT_NE(vector, Vector2(4, 0));
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

	TEST(Vector2, Normalize)
	{
		const Vector2 vector(1, 2);
		const Vector2 result = vector.Normalize();
		EXPECT_FLOAT_EQ(result.Magnitude(), 1);
		EXPECT_GT(result.y, result.x);
	}

	TEST(Vector2, Magnitude)
	{
		const Vector2 vector(5, 12);
		const float result = vector.Magnitude();
		EXPECT_FLOAT_EQ(result, 13);
	}

	TEST(Vector2, Dot)
	{
		const Vector2 vector(1, 2);
		const float result = vector.Dot(Vector2(1, 5));
		EXPECT_FLOAT_EQ(result, 11);
	}

	TEST(Vector2, Orthoginal)
	{
		EXPECT_TRUE(Vector2(5, 5).IsOrthoginal(Vector2(5, -5)));
		EXPECT_FALSE(Vector2(45, 23).IsOrthoginal(Vector2(32, 2)));
	}

	TEST(Vector2, Acute)
	{
		EXPECT_TRUE(Vector2(1, 2).IsAngleAcute(Vector2(1, 5)));
		EXPECT_FALSE(Vector2(3, 2).IsAngleAcute(Vector2(1, -10)));
	}

	TEST(Vector2, Obtuse)
	{
		EXPECT_FALSE(Vector2(1, 2).IsAngleObtuse(Vector2(1, 5)));
		EXPECT_TRUE(Vector2(3, 2).IsAngleObtuse(Vector2(1, -10)));
	}

	TEST(Vector2, Orthogonalize)
	{
		Vector2 a = Vector2(2, 2);
		Vector2 b = Vector2(1, 2);

		EXPECT_FALSE(a.IsOrthoginal(b));
		Vector2 result = b.Orthogonalize(a);
		EXPECT_TRUE(a.IsOrthoginal(result));
	}

	TEST(Vector2, OrthogonalizeSet)
	{
		Vector2 x1 = Vector2(2, 2);
		Vector2 x2 = Vector2(1, 2);

		EXPECT_FALSE(x1.IsOrthoginal(x2));
		x2 = x2.Orthogonalize(x1);
		EXPECT_TRUE(x1.IsOrthoginal(x2));
	}

	TEST(Vector2, Project)
	{
		Vector2 v = Vector2(-4, 2);
		Vector2 u = Vector2(3, 1);
		Vector2 projection = v.ProjectOnto(u);
		EXPECT_EQ(projection, Vector2(-3, -1));
	}
}