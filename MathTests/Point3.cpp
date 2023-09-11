#include <gtest/gtest.h>
#include <blPoint3.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(Point3, Initialization)
	{
		Point3 point = Point3(5, 8, 10.5);

		EXPECT_FLOAT_EQ(point.x, point[0]);
		EXPECT_FLOAT_EQ(point.y, point[1]);
		EXPECT_FLOAT_EQ(point.z, point[2]);

		EXPECT_FLOAT_EQ(point.x, point.data[0]);
		EXPECT_FLOAT_EQ(point.y, point.data[1]);
		EXPECT_FLOAT_EQ(point.z, point.data[2]);

		EXPECT_FLOAT_EQ(point.data[0], point[0]);
		EXPECT_FLOAT_EQ(point.data[1], point[1]);
		EXPECT_FLOAT_EQ(point.data[2], point[2]);
	}

	TEST(Point3, Equality)
	{
		Point3 point = Point3(5, 8, 10.5f);

		EXPECT_EQ(point, Point3(5, 8, 10.5f));
		EXPECT_NE(point, Point3(15.0f, 23.0f, 10.8f));
		EXPECT_NE(point, Point3(5, 8, 10.8f));
	}

	TEST(Point3, SubtractForVector)
	{
		EXPECT_EQ(Vector3(2, -1, 10), Point3(2, 5, 10) - Point3(4, 4, 20));
		EXPECT_EQ(Vector3(3, 9, 0), Point3(-5, -5, 10) - Point3(-2, 4, 10));
		EXPECT_EQ(Vector3(-150, -25, -7), Point3(100, 50, -7) - Point3(-50, 25, -14));
		EXPECT_EQ(Vector3(0, 0, 0), Point3(20, 40, 23.5) - Point3(20, 40, 23.5));

	}

	TEST(Point3, AddVector)
	{
		EXPECT_EQ(Point3(6, 9, 30), Point3(2, 5, 10) + Vector3(4, 4, 20));
		EXPECT_EQ(Point3(-5, 130, -110), Point3(-5, 100, -100) + Vector3(0, 30, -10));
	}

	TEST(Point3, Vector3Ref)
	{
		Point3 point(3, 4, 5);
		/*Vector3& vector = point;

		EXPECT_EQ(Vector3(3, 4, 5), vector);

		vector.x = 100;
		vector.y = 200;
		vector.z = 300;

		EXPECT_EQ(point.x, 100);
		EXPECT_EQ(point.y, 200);
		EXPECT_EQ(point.z, 300);*/
	}
}