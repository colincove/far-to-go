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
		EXPECT_TRUE(NearEqual(result, Vector3(2.73055795f, 1.88665545f, 0.99226231f)));
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

	TEST(Transformations, Transition)
	{
		const Vector3 v(0.15f, 0.15f, 0.15f);
		const CartesianCoordinates frame(CartesianCoordinates::FromAxes(
			Vector3(1, 1, 0.75), Vector3(0, 1.2, 0.5), Vector3(-1, 1, 2)));

		const Vector3 result(Transition(v, frame));

		EXPECT_FLOAT_EQ(v.Magnitude(), result.Magnitude());
	}

	TEST(Transformations, TransitionMatrix)
	{
		const Vector4 v(Vector4::Vector3D(0.15f, 0.15f, 0.15f));
		const CartesianCoordinates frame(CartesianCoordinates::FromAxes(
			Vector3(1, 1, 0.75), Vector3(0, 1.2, 0.5), Vector3(-1, 1, 2)));

		const Vector4 origin(Vector4::Point3D(2, 4, 0));
		const Matrix4x4 transitionMatrix(TransitionMatrix(frame, origin));

		const Vector4 result(Matrix4x4::Transform(transitionMatrix, v));

		EXPECT_FLOAT_EQ(v.Magnitude(), result.Magnitude());
	}

	TEST(Transformations, MultipleTransitionMatrix)
	{
		const Vector4 v(Vector4::Vector3D(0.15f, 0.15f, 0.15f));
		const CartesianCoordinates frameA(CartesianCoordinates::FromAxes(
			Vector3(1, 1, 0.75), Vector3(0, 1.2, 0.5), Vector3(-1, 1, 2)));
		const CartesianCoordinates frameB(CartesianCoordinates::FromAxes(
			Vector3(1.4f, 0.3f, 0.15f), Vector3(0, 2, 2), Vector3(-1, 1, 2)));

		const Vector4 originA(Vector4::Point3D(2, 4, 0));
		const Vector4 originB(Vector4::Point3D(1, 0, 0.5f));
		const Matrix4x4 transitionMatrixA(TransitionMatrix(frameA, originA));
		const Matrix4x4 transitionMatrixB(TransitionMatrix(frameB, originB));

		//combine multiple frame changes into a single matrix
		const Matrix4x4 transitionMatrix(transitionMatrixA * transitionMatrixB);

		const Vector4 result(Matrix4x4::Transform(transitionMatrix, v));

		EXPECT_FLOAT_EQ(v.Magnitude(), result.Magnitude());
	}

	TEST(Transformations, TransitionInverseMatrix)
	{
		const Vector4 v(Vector4::Vector3D(0.15f, 0.15f, 0.15f));
		const CartesianCoordinates frame(CartesianCoordinates::FromAxes(
			Vector3(1, 1, 0.75), Vector3(0, 1.2, 0.5), Vector3(-1, 1, 2)));

		const Vector4 origin(Vector4::Point3D(2, 4, 0));
		const Matrix4x4 transitionMatrix(TransitionMatrix(frame, origin));
		const Matrix4x4 transitionMatrixInverse(transitionMatrix.Inverse());

		const Vector4 transformVector(Matrix4x4::Transform(transitionMatrix, v));
		const Vector4 result(Matrix4x4::Transform(transitionMatrixInverse, transformVector));

		EXPECT_FLOAT_EQ(v.Magnitude(), result.Magnitude());
		EXPECT_TRUE(NearEqual(v, result));
	}
}