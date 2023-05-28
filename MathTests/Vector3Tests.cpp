#include <gtest/gtest.h>
#include "../Math/Vector3.h"

using Vector3 = BoulderLeaf::Math::Vector3<float>;

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

	EXPECT_TRUE(vector == Vector3(0, 4, 5));
	EXPECT_FALSE(vector == Vector3(0, 6, 10));

	EXPECT_TRUE(vector != Vector3(-10.6f, 4.5f, 5.2f));
	EXPECT_FALSE(vector != Vector3(0, 4, 5));
}

TEST(Vector3, Cross) 
{
	Vector3 vector(1, 2, 3);
	Vector3 result = vector.Cross(Vector3(1, 5, 7));
	EXPECT_EQ(result, Vector3(-1, -4, 3));
}

TEST(Vector3, Normalize)
{
	Vector3 vector(1, 2, 3);
	Vector3 result = vector.Normalize();
	EXPECT_EQ(result.Magnitude(), 1);
	EXPECT_GT(result.y, result.x);
	EXPECT_GT(result.z, result.y);
}

TEST(Vector3, Magnitude)
{
	Vector3 vector(2, 4, 4);
	float result = vector.Magnitude();
	EXPECT_EQ(result, 6);
}

TEST(Vector3, Dot)
{
	Vector3 vector(1, 2, 3);
	float result = vector.Dot(Vector3(1, 5, 7));
	EXPECT_EQ(result, 32);
}