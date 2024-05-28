#include <gtest/gtest.h>
#include <blQuaternion.inl>
#include <blVector3.inl>

namespace BoulderLeaf::Math::Test
{
	TEST(Quaternion, Multiplication)
	{
		Quaternion q1(3.0f, 2.0f, 4.0f, 1.0f);
		Quaternion q2(1.0f, 3.0f, 5.0f, 2.0f);
		Quaternion result = q1 * q2;
		EXPECT_EQ(result, Quaternion(-25, 14, 18, 5));
	}

	TEST(Quaternion, RotationMatrix3x3)
	{
		Quaternion q(5, 8, 2, 3);
		q.NormalizeInPlace();
		Matrix3x3 result(Quaternion::RotationMatrix3x3(q));

		EXPECT_TRUE(NearEqual(result, Matrix3x3(
			0.7450981f, 0.0196078f, 0.6666667f,
			0.6078432f, -0.4313726f, -0.6666667f,
			0.2745098f, 0.9019608f, -0.3333333f
		)));
	}

	TEST(Quaternion, RotationMatrix4x4)
	{
		Quaternion q(5, 8, 2, 3);
		q.NormalizeInPlace();
		Matrix4x4 result(Quaternion::RotationMatrix4x4(q));

		EXPECT_TRUE(NearEqual(result, Matrix4x4(
			0.7450981f, 0.0196078f, 0.6666667f, 0,
			0.6078432f, -0.4313726f, -0.6666667f, 0,
			0.2745098f, 0.9019608f, -0.3333333f, 0,
			0,0,0,0
		)));
	}
}