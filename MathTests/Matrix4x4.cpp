#include <gtest/gtest.h>
#include <blMatrix4x4.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(blMatrix4x4, Initialization)
	{
		Matrix4x4 m(
			1,  2,  3,  4,
			5,  6,  7,  8,
			9,  10, 11, 12,
			13, 14, 15, 16);

		EXPECT_FLOAT_EQ(m.m00, 1);
		EXPECT_FLOAT_EQ(m.m01, 2);
		EXPECT_FLOAT_EQ(m.m02, 3);
		EXPECT_FLOAT_EQ(m.m03, 4);
		EXPECT_FLOAT_EQ(m.m10, 5);
		EXPECT_FLOAT_EQ(m.m11, 6);
		EXPECT_FLOAT_EQ(m.m12, 7);
		EXPECT_FLOAT_EQ(m.m13, 8);
		EXPECT_FLOAT_EQ(m.m20, 9);
		EXPECT_FLOAT_EQ(m.m21, 10);
		EXPECT_FLOAT_EQ(m.m22, 11);
		EXPECT_FLOAT_EQ(m.m23, 12);
		EXPECT_FLOAT_EQ(m.m30, 13);
		EXPECT_FLOAT_EQ(m.m31, 14);
		EXPECT_FLOAT_EQ(m.m32, 15);
		EXPECT_FLOAT_EQ(m.m33, 16);
	}

	TEST(blMatrix4x4, Rows)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_EQ(m.GetRow(0), Matrix4x4::RowVector(1, 2, 3, 4));
		EXPECT_EQ(m.GetRow(1), Matrix4x4::RowVector(5, 6, 7, 8));
		EXPECT_EQ(m.GetRow(2), Matrix4x4::RowVector(9, 10, 11, 12));
		EXPECT_EQ(m.GetRow(3), Matrix4x4::RowVector(13, 14, 15, 16));
	}

	TEST(blMatrix4x4, Columns)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_EQ(m.GetColumn(0), Matrix4x4::ColumnVector(1, 5, 9, 13));
		EXPECT_EQ(m.GetColumn(1), Matrix4x4::ColumnVector(2, 6, 10, 14));
		EXPECT_EQ(m.GetColumn(2), Matrix4x4::ColumnVector(3, 7, 11, 15));
		EXPECT_EQ(m.GetColumn(3), Matrix4x4::ColumnVector(4, 8, 12, 16));
	}

	TEST(blMatrix4x4, GetElement)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_FLOAT_EQ(m.GetElement(0, 0), 1);
		EXPECT_FLOAT_EQ(m.GetElement(0, 1), 2);
		EXPECT_FLOAT_EQ(m.GetElement(0, 2), 3);
		EXPECT_FLOAT_EQ(m.GetElement(0, 3), 4);
		EXPECT_FLOAT_EQ(m.GetElement(1, 0), 5);
		EXPECT_FLOAT_EQ(m.GetElement(1, 1), 6);
		EXPECT_FLOAT_EQ(m.GetElement(1, 2), 7);
		EXPECT_FLOAT_EQ(m.GetElement(1, 3), 8);
		EXPECT_FLOAT_EQ(m.GetElement(2, 0), 9);
		EXPECT_FLOAT_EQ(m.GetElement(2, 1), 10);
		EXPECT_FLOAT_EQ(m.GetElement(2, 2), 11);
		EXPECT_FLOAT_EQ(m.GetElement(2, 3), 12);
		EXPECT_FLOAT_EQ(m.GetElement(3, 0), 13);
		EXPECT_FLOAT_EQ(m.GetElement(3, 1), 14);
		EXPECT_FLOAT_EQ(m.GetElement(3, 2), 15);
		EXPECT_FLOAT_EQ(m.GetElement(3, 3), 16);
	}

	TEST(blMatrix4x4, IndexOperator)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_FLOAT_EQ(m[0], 1);
		EXPECT_FLOAT_EQ(m[1], 2);
		EXPECT_FLOAT_EQ(m[2], 3);
		EXPECT_FLOAT_EQ(m[3], 4);
		EXPECT_FLOAT_EQ(m[4], 5);
		EXPECT_FLOAT_EQ(m[5], 6);
		EXPECT_FLOAT_EQ(m[6], 7);
		EXPECT_FLOAT_EQ(m[7], 8);
		EXPECT_FLOAT_EQ(m[8], 9);
		EXPECT_FLOAT_EQ(m[9], 10);
		EXPECT_FLOAT_EQ(m[10], 11);
		EXPECT_FLOAT_EQ(m[11], 12);
		EXPECT_FLOAT_EQ(m[12], 13);
		EXPECT_FLOAT_EQ(m[13], 14);
		EXPECT_FLOAT_EQ(m[14], 15);
		EXPECT_FLOAT_EQ(m[15], 16);
	}

	TEST(blMatrix4x4, GetElementMutable)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		m.GetElementMutable(0, 0) = 20.5f;
		EXPECT_FLOAT_EQ(m.GetElement(0, 0), 20.5f);

		m.GetElementMutable(2, 1) = 5.75f;
		EXPECT_FLOAT_EQ(m.GetElement(2, 1), 5.75f);
	}

	TEST(blMatrix4x4, AdditionInPlace)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		m += 5;

		EXPECT_EQ(m, 
			Matrix4x4(
			6, 7, 8, 9,
			10, 11, 12, 13,
			14, 15, 16, 17,
			18, 19, 20, 21));
	}

	TEST(blMatrix4x4, Addition)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_EQ(m + 5,
			Matrix4x4(
				6, 7, 8, 9,
				10, 11, 12, 13,
				14, 15, 16, 17,
				18, 19, 20, 21));
	}

	TEST(blMatrix4x4, MatrixAddition)
	{
		Matrix4x4 m1(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		Matrix4x4 m2(
			0, 0, 0, 4,
			8, 1, 3, 8,
			9, 0, 11, 12,
			13, 14, 15, 16);

		Matrix4x4 m = m1 + m2;

		EXPECT_EQ(m,
			Matrix4x4(
				1, 2, 3, 8,
				13, 7, 10, 16,
				18, 10, 22, 24,
				26, 28, 30, 32));
	}

	TEST(blMatrix4x4, SubtractionInPlace)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		m -= 1;

		EXPECT_EQ(m,
			Matrix4x4(
				0, 1, 2, 3,
				4, 5, 6, 7,
				8, 9, 10, 11,
				12, 13, 14, 15));
	}

	TEST(blMatrix4x4, Subtraction)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_EQ(m - 1,
			Matrix4x4(
				0, 1, 2, 3,
				4, 5, 6, 7,
				8, 9, 10, 11,
				12, 13, 14, 15));
	}

	TEST(blMatrix4x4, MatrixSubtraction)
	{
		Matrix4x4 m1(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		Matrix4x4 m2(
			0, 0, 0, 4,
			8, 1, 3, 8,
			9, 0, 11, 12,
			13, 14, 15, 16);

		Matrix4x4 m = m1 - m2;

		EXPECT_EQ(m,
			Matrix4x4(
				1, 2, 3, 0,
				-3, 5, 4, 0,
				0, 10, 0, 0,
				0, 0, 0, 0));
	}

	TEST(blMatrix4x4, MultiplicationInPlace)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		m *= 2;

		EXPECT_EQ(m,
			Matrix4x4(
				2, 4, 6, 8,
				10, 12, 14, 16,
				18, 20, 22, 24,
				26, 28, 30, 32));
	}

	TEST(blMatrix4x4, Multiplication)
	{
		Matrix4x4 m(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_EQ(m * 2,
			Matrix4x4(
				2, 4, 6, 8,
				10, 12, 14, 16,
				18, 20, 22, 24,
				26, 28, 30, 32));
	}

	TEST(blMatrix4x4, MatrixMultiplication)
	{
		Matrix4x4 m1(
			1, 2, 3, 4,
			5, 6, 7, 8,
			9, 10, 11, 12,
			13, 14, 15, 16);

		Matrix4x4 m2(
			0, 0, 0, 4,
			8, 1, 3, 8,
			9, 0, 11, 12,
			2, 2, 2, 2);

		Matrix4x4 result = m1 * m2;

		EXPECT_EQ(result,
			Matrix4x4(
				51, 10, 47, 64,
				127,22, 111, 168,
				203, 34, 175, 272,
				279, 46, 239, 376));
	}

	TEST(blMatrix4x4, DevisionInPlace)
	{
		Matrix4x4 m(
			10, 10, 20, 30,
			60, 2, 1, 0,
			10, 10, 11, 12,
			13, 14, 15, 16);

		m /= 2;

		EXPECT_EQ(m,
			Matrix4x4(
				5, 5, 10, 15,
				30, 1, 0.5f, 0,
				5, 5, 5.5f, 6,
				6.5f, 7, 7.5f, 8));
	}

	TEST(blMatrix4x4, Devision)
	{
		Matrix4x4 m(
			10, 10, 20, 30,
			60, 2, 1, 0,
			10, 10, 11, 12,
			13, 14, 15, 16);

		EXPECT_EQ(m / 2,
			Matrix4x4(
				5, 5, 10, 15,
				30, 1, 0.5f, 0,
				5, 5, 5.5f, 6,
				6.5f, 7, 7.5f, 8));
	}

	TEST(blMatrix4x4, Transpose)
	{
		Matrix4x4 m(
			5, 10, 3, 2,
			9, 2, 2, 3,
			8, 9, 4, 3,
			1, 1, 2, 2
		);

		Matrix4x4 mt(m.Transpose());
		EXPECT_EQ(mt, Matrix4x4(
			5, 9, 8, 1,
			10, 2, 9, 1,
			3, 2, 4, 2,
			2, 3, 3, 2
		));
	}

	TEST(blMatrix4x4, Identity)
	{
		Matrix4x4 m(
			5, 10, 3, 2,
			9, 2, 2, 3,
			8, 9, 4, 3,
			1, 1, 2, 2
		);

		Matrix4x4 result(m * Matrix4x4::Identity());
		EXPECT_EQ(m, result);
	}
}