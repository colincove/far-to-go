#include <gtest/gtest.h>
#include <blMesh.h>

namespace
{
	using namespace BoulderLeaf::Graphics;
	using namespace BoulderLeaf;

	StandardMesh GetTestMesh()
	{
		StandardMesh mesh(5, 9);

		const StandardVertex v0 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v1 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v2 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v3 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v4 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });

		mesh.GetVertexMutable(0) = v0;
		mesh.GetVertexMutable(1) = v1;
		mesh.GetVertexMutable(2) = v2;
		mesh.GetVertexMutable(3) = v3;
		mesh.GetVertexMutable(4) = v4;

		mesh.GetIndexMutable(0) = 3;
		mesh.GetIndexMutable(1) = 1;
		mesh.GetIndexMutable(2) = 1;
		mesh.GetIndexMutable(3) = 2;
		mesh.GetIndexMutable(4) = 4;
		mesh.GetIndexMutable(5) = 3;
		mesh.GetIndexMutable(6) = 2;
		mesh.GetIndexMutable(7) = 1;
		mesh.GetIndexMutable(8) = 1;

		return mesh;
	}
}

namespace BoulderLeaf::Graphics
{
	TEST(Mesh, Initialize)
	{
		StandardMesh mesh(GetTestMesh());

		const StandardVertex v0 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v1 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v2 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v3 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v4 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });

		const StandardVertex vt = mesh.GetVertex(1);

		EXPECT_EQ(mesh.GetVertex(0), v0);
		EXPECT_EQ(mesh.GetVertex(1), v1);
		EXPECT_EQ(mesh.GetVertex(2), v2);
		EXPECT_EQ(mesh.GetVertex(3), v3);
		EXPECT_EQ(mesh.GetVertex(4), v4);
	}

	TEST(Mesh, Copy)
	{
		StandardMesh mesh(GetTestMesh());

		const StandardMesh copy = mesh;

		for (int i = 0; i < mesh.GetIndexCount(); ++i)
		{
			EXPECT_EQ(mesh.GetIndex(i), copy.GetIndex(i));
		}

		for (int i = 0; i < mesh.GetVertexCount(); ++i)
		{
			EXPECT_EQ(mesh.GetVertex(i), copy.GetVertex(i));
		}

		mesh.GetVertexMutable(2) = StandardVertex({ Math::Vector3(-5.0f, -3.0f, -8.0f), Math::Vector3(8, 8, 8), Math::Vector4(8, 1, 1, 1), Math::Vector2(6, 6) });

		EXPECT_NE(mesh.GetVertex(2), copy.GetVertex(2));
	}

	TEST(Mesh, Move)
	{
		StandardMesh mesh(GetTestMesh());

		const StandardMesh moved = std::move(mesh);

		EXPECT_FALSE(mesh.IsValid());
		EXPECT_TRUE(moved.IsValid());

		EXPECT_EQ(moved.GetVertex(3), GetTestMesh().GetVertex(3));
	}
}