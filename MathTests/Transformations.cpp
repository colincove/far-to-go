#include <gtest/gtest.h>
#include <blMatrix3x3.inl>
#include <blMatrix4x4.inl>
#include <blMath3D.inl>
#include <blVector4.inl>
#include <cmath>

namespace BoulderLeaf::Math::Test
{
	TEST(Transformations, ScalingMatrix)
	{
		const Matrix3x3 scalingMatrix = Matrix3x3::ScalingMatrix(Vector3(2, 2, 1));
		const Vector3 origin(5, 5, 5);
		const Vector3 transformation(Matrix3x3::Transform(scalingMatrix, origin));
		EXPECT_EQ(transformation, Vector3(10, 10, 5));
	}

	TEST(Transformations, RotateAboutAxis)
	{
		//rotate v about n by angle a
		const Vector3 v(2, 2, 2);
		const Vector3 n(Vector3(4, 8, 2).Normalize());
		const float a(45);

		const Vector3 result(RotateAboutAxisDegrees(v, n, a));
		EXPECT_TRUE(NearEqual(result, Vector3(2.73055795, 1.88665545, 0.99226231)));
	}

	TEST(Transformations, RotationMatrix)
	{
		//rotate v about n by angle a
		const Vector3 origin(2, 2, 2);
		const Vector3 axis(Vector3(4, 8, 2).Normalize());
		const float radians(ToRadians(45));

		const Matrix3x3 rotationMatrix(Matrix3x3::RotationMatrix(axis, radians));
		const Vector3 transformation(Matrix3x3::Transform(rotationMatrix, origin));
		EXPECT_TRUE(NearEqual(transformation, Vector3(2.73055795, 1.88665545, 0.99226231)));
	}

	TEST(Transformations, Translation)
	{
		//rotate v about n by angle a
		const Vector3 translation(2, 2, 2);
		const Vector4 point(Vector4::Point3D(4, 8, 2));

		const Matrix4x4 translationMatrix(Matrix4x4::TranslationMatrix(translation));
		const Vector4 transformation(Matrix4x4::Transform(translationMatrix, point));
		EXPECT_TRUE(NearEqual(transformation, Vector4::Point3D(6, 10, 4)));
	}
}