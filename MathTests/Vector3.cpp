#include <gtest/gtest.h>
#include <blVector3.inl>

namespace BoulderLeaf::Math
{
	TEST(Vector3, Initialization)
	{
		Vector3 vector(0, 4, 5);

		EXPECT_EQ(vector.x, 0);
		EXPECT_EQ(vector.y, 4);
		EXPECT_EQ(vector.z, 5);
		EXPECT_EQ(vector.x, vector[0]);
		EXPECT_EQ(vector.y, vector[1]);
		EXPECT_EQ(vector.z, vector[2]);
		EXPECT_EQ(vector.data[0], vector[0]);
		EXPECT_EQ(vector.data[1], vector[1]);
		EXPECT_EQ(vector.data[2], vector[2]);
	}

	TEST(Vector3, Equality)
	{
		Vector3 vector(0, 4, 5);

		EXPECT_EQ(vector, Vector3(0, 4, 5));
		EXPECT_NE(vector, Vector3(0, 6, 10));

		EXPECT_TRUE(vector != Vector3(-10.6f, 4.5f, 5.2f));
		EXPECT_FALSE(vector != Vector3(0, 4, 5));
	}

	TEST(Vector3, Multiply)
	{
		Vector3 vector(10, 5.5f, 2);
		Vector3 result = vector * Vector3(2, 3, 1);
		EXPECT_EQ(result, Vector3(20, 16.5f, 2));
		EXPECT_EQ(vector * 3.5f, Vector3(35, 19.25, 7));

		vector = Vector3(10, 5.5f, 2);
		vector *= Vector3(2, 2, 5);
		EXPECT_EQ(vector, Vector3(20, 11, 10));

		vector = Vector3(10, 5.5f, 2);
		vector *= 3.0f;
		EXPECT_EQ(vector, Vector3(30, 16.5, 6));
	}

	TEST(Vector3, Devide)
	{
		Vector3 vector(20, 30, 5);
		Vector3 result = vector / Vector3(2, 3, 2);
		EXPECT_EQ(result, Vector3(10, 10, 2.5f));
		EXPECT_EQ(vector / 2.0f, Vector3(10, 15, 2.5f));
		EXPECT_EQ(vector / Vector3(2, 2, 2), Vector3(10, 15, 2.5f));

		vector = Vector3(20, 30, 5);
		vector /= 2.0f;
		EXPECT_EQ(vector, Vector3(10, 15, 2.5f));

		vector = Vector3(20, 30, 5);
		vector /= Vector3(2, 3, 5);
		EXPECT_EQ(vector, Vector3(10, 10, 1));
	}

	TEST(Vector3, Addition)
	{
		Vector3 vector(10, 5.5f, 2);
		Vector3 result = vector + Vector3(2, 3, 1);
		EXPECT_EQ(result, Vector3(12, 8.5f, 3));

		vector = Vector3(10, 5.5f, 2);
		vector += Vector3(5, 5, 2);
		EXPECT_EQ(vector, Vector3(15, 10.5f, 4));

		vector = Vector3(10, 5.5f, 2);
		vector += 2.0f;
		EXPECT_EQ(vector, Vector3(12, 7.5f, 4));
	}

	TEST(Vector3, Subtract)
	{
		Vector3 vector(10, 5.5f, 2);
		const Vector3 result = vector - Vector3(2, 3, 1);
		EXPECT_EQ(result, Vector3(8, 2.5f, 1));

		vector = Vector3(10, 5.5f, 2);
		vector -= Vector3(5, 5, 2);
		EXPECT_EQ(vector, Vector3(5, 0.5f, 0));
	}

	TEST(Vector3, Cross)
	{
		const Vector3 vector(1, 2, 3);
		const Vector3 result = vector.Cross(Vector3(1, 5, 7));
		EXPECT_EQ(result, Vector3(-1, -4, 3));
	}

	TEST(Vector3, Normalize)
	{
		const Vector3 vector(1, 2, 3);
		const Vector3 result = vector.Normalize();
		const float resultMagnitude = result.Magnitude();
		EXPECT_FLOAT_EQ(resultMagnitude, 1.0f);
		EXPECT_GT(result.y, result.x);
		EXPECT_GT(result.z, result.y);
	}

	TEST(Vector3, Magnitude)
	{
		const Vector3 vector(2, 4, 4);
		const float resultMagnitude = vector.Magnitude();
		EXPECT_FLOAT_EQ(resultMagnitude, 6);
	}

	TEST(Vector3, Dot)
	{
		const Vector3 vector(1, 2, 3);
		const float result = vector.Dot(Vector3(1, 5, 7));
		EXPECT_FLOAT_EQ(result, 32);
	}

	TEST(Vector3, Orthoginal)
	{
		EXPECT_TRUE(Vector3(5, 5, 0).IsOrthoginal(Vector3(5, -5, 0)));
		EXPECT_TRUE(Vector3(1, 2, 3).IsOrthoginal(Vector3(1, 4, -3)));
		EXPECT_FALSE(Vector3(45, 23, 2).IsOrthoginal(Vector3(32, 2, 3)));
	}

	TEST(Vector3, Acute)
	{
		EXPECT_TRUE(Vector3(1, 2, 3).IsAngleAcute(Vector3(1, 5, -3)));
		EXPECT_FALSE(Vector3(3, 2, 3).IsAngleAcute(Vector3(1, -10, 5)));
	}

	TEST(Vector3, Obtuse)
	{
		EXPECT_FALSE(Vector3(1, 2, 3).IsAngleObtuse(Vector3(1, 5, -3)));
		EXPECT_TRUE(Vector3(3, 2, 3).IsAngleObtuse(Vector3(1, -10, 5)));
	}
}