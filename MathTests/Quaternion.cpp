#include <gtest/gtest.h>
#include <blQuaternion.inl>
#include <blVector3.inl>
#include "blMath.inl"

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
			0,0,0,1
		)));

		EXPECT_TRUE(result.HasValidInverse());
	}

	TEST(Quaternion, FromEuler)
	{
		const Vector3 eulerAngles(ToRadians(45), ToRadians(10), ToRadians(8));
		const Quaternion result(Quaternion::FromEuler(eulerAngles));
		const Quaternion expected(0.920448601f, 0.374681681f, 0.106918328f, 0.0309295245f);
		EXPECT_TRUE(IsNearEqual(std::begin(result.data), std::begin(expected.data), Quaternion::k_size));
	}

	TEST(Quaternion, ToEuler)
	{
		const Quaternion quaternion(0.920448601f, 0.374681681f, 0.106918328f, 0.0309295245f);
		const Vector3 result(quaternion.ToEuler());
		EXPECT_TRUE(NearEqual(result, Vector3(ToRadians(45), ToRadians(10), ToRadians(8))));
	}

	TEST(Quaternion, LookAt)
	{
		Vector4 from = Vector4::Point3D(0, 0, 0);
		Vector4 to = Vector4::Point3D(3, 3, 3);

		constexpr Vector4 up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		const Matrix4x4 matrixLookAt = Matrix4x4::LookAtLH(from, to , up);

		const Quaternion rotationQuaternion = Quaternion::LookAt(Vector3(from.x, from.y, from.z), Vector3(to.x, to.y, to.z));
		const Matrix4x4 rotationQuaternionMatrix = Quaternion::RotationMatrix4x4(rotationQuaternion);

		//strange result. I punched these values into a visualizer online. 
		//matrixLookAtTransformed looked wrong. It had no y value. 
		//quternionLookAtTransformed looked correct though......
		const Vector4 matrixLookAtTransformed = Vector4::Forward() * matrixLookAt;
		const Vector4 quternionLookAtTransformed = Vector4::Forward() * rotationQuaternionMatrix;

		EXPECT_TRUE(NearEqual(matrixLookAt, rotationQuaternionMatrix));
	}
}
