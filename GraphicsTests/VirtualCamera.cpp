#include <gtest/gtest.h>
#include <blVirtualCamera.h>

namespace BoulderLeaf::Graphics
{
	TEST(VirtualCamera, Initialize)
	{
		VirtualCamera camera(5, 15, 0.8f, 0.66f);
		VirtualCamera cameraDefault;

		EXPECT_FLOAT_EQ(camera.GetNearPlane(), 5);
		EXPECT_FLOAT_EQ(camera.GetFarPlane(), 15);
		EXPECT_FLOAT_EQ(camera.GetViewAngle(), 0.8f);
		EXPECT_FLOAT_EQ(camera.GetAspectRatio(), 0.66f);
	}

	TEST(VirtualCamera, Project)
	{
		const VirtualCamera camera(5, 15, 0.3f, 0.66f);
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