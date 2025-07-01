#include <blDX12.h>
#include <gtest/gtest.h>
#include "..\Graphics\blMesh.h"
#include <blDx12VertexData.h>

namespace
{
	using namespace BoulderLeaf::Graphics;
	using namespace BoulderLeaf;

	StandardMesh GetTestMesh()
	{
		StandardMesh mesh(5, 9);

		const StandardVertex v0 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v1 = StandardVertex({ Math::Vector3(-1.0f, -5.0f, -6.0f), Math::Vector3(1, 1, 1), Math::Vector3(), Math::Vector4(1, 0.5f, 0.1f, 0), Math::Vector2(2, 2) });
		const StandardVertex v2 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v3 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });
		const StandardVertex v4 = StandardVertex({ Math::Vector3(-1.0f, -1.0f, -1.0f), Math::Vector3(), Math::Vector3(), Math::Vector4(1, 1, 1, 1), Math::Vector2() });

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

namespace BoulderLeaf::Graphics::DX12::Test
{
	TEST(DX12, StorageConversion)
	{
		StandardMesh mesh = GetTestMesh();
		Mesh dx12Mesh = Mesh(blMeshStorage::To<StandardVertex, Vertex>(mesh.GetStorage()));

		const Vertex v1 = Vertex(
			XMFLOAT3(-1.0f, -5.0f, -1.0f),
			XMFLOAT3(1, 1, 1),
			XMFLOAT3(),
			XMFLOAT4(1, 0.5f, 0.1f, 0),
			XMFLOAT2(2, 2)
		);

		EXPECT_EQ(dx12Mesh.GetVertex(1), v1);
	} 
}