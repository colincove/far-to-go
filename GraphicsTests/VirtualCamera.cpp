#include <gtest/gtest.h>
#include <blVirtualCamera.h>

namespace BoulderLeaf::Graphics
{
	TEST(VirtualCamera, Initialize)
	{
		const float nearPlane = 5;
		const float farPlane = 15;
		const float viewAngle = 0.3f;
		const float aspectRatio = 0.66f;

		const VirtualCamera camera(
			nearPlane,
			farPlane,
			viewAngle,
			aspectRatio);

		VirtualCamera cameraDefault;

		EXPECT_FLOAT_EQ(camera.GetNearPlane(), nearPlane);
		EXPECT_FLOAT_EQ(camera.GetFarPlane(), farPlane);
		EXPECT_FLOAT_EQ(camera.GetViewAngle(), viewAngle);
		EXPECT_FLOAT_EQ(camera.GetAspectRatio(), aspectRatio);
	}

	TEST(VirtualCamera, Project)
	{
		const float nearPlane = 5;
		const float farPlane = 15;
		const float viewAngle = 0.3f;
		const float aspectRatio = 0.66f;

		const VirtualCamera camera(
			nearPlane, 
			farPlane, 
			viewAngle, 
			aspectRatio);

		const Matrix4x4 projectionMatrix(camera.GetProjectionMatrix());

		Vector4 vertice(Vector4::Point3D(0.5f, 1, 10));
		Vector4 projectedVertice(camera.ProjectVertice(vertice));
		Vector4 matrixProjectedVertice(camera.ApplyProjectionMatrix(projectionMatrix, vertice));
		EXPECT_TRUE(camera.IsInsideFustrum(projectedVertice));
		EXPECT_TRUE(camera.IsInsideFustrum(matrixProjectedVertice));

		vertice = Vector4::Point3D(0.5f, 1, 16);
		projectedVertice = camera.ProjectVertice(vertice);
		matrixProjectedVertice = camera.ApplyProjectionMatrix(projectionMatrix, vertice);
		EXPECT_FALSE(camera.IsInsideFustrum(projectedVertice));
		EXPECT_FALSE(camera.IsInsideFustrum(matrixProjectedVertice));

		vertice = Vector4::Point3D(2.0f, 0.1f, 14);
		projectedVertice = camera.ProjectVertice(vertice);
		matrixProjectedVertice = camera.ApplyProjectionMatrix(projectionMatrix, vertice);
		EXPECT_FALSE(camera.IsInsideFustrum(projectedVertice));
		EXPECT_FALSE(camera.IsInsideFustrum(matrixProjectedVertice));

		vertice = Vector4::Point3D(1.0f, 0.5f, 6);
		projectedVertice = camera.ProjectVertice(vertice);
		matrixProjectedVertice = camera.ApplyProjectionMatrix(projectionMatrix, vertice);
		EXPECT_FALSE(camera.IsInsideFustrum(projectedVertice));
		EXPECT_FALSE(camera.IsInsideFustrum(matrixProjectedVertice));

		vertice = Vector4::Point3D(1.0f, 0.5f, 14);
		projectedVertice = camera.ProjectVertice(vertice);
		matrixProjectedVertice = camera.ApplyProjectionMatrix(projectionMatrix, vertice);
		EXPECT_TRUE(camera.IsInsideFustrum(projectedVertice));
		EXPECT_TRUE(camera.IsInsideFustrum(matrixProjectedVertice));
	}
}