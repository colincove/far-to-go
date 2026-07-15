#include <gtest/gtest.h>
#include <blCompositeMesh.h>
#include <Resources/blResourcesExprimental.h>
#include <blBuffer.h>

using namespace BoulderLeaf;

TEST(CompositeMesh, Resource)
{
	using namespace Graphics;

	blResourceContainerPool pool;
	blResourceContainer* container = pool.GetContainer(
		1000000 * 256, // 256mb
		10000
	);

	BufferDescription desc =
	{
		BufferFormat::DX12,
		{ {"A", BufferElementType::Float} }
	};

	blResourceHandleOfType<blBufferDescriptionResource> descResource =
		container->CreateResourceOfTypeWithDynamicSize<blBufferDescriptionResource>(L"BufferDescriptionResource", desc);

	struct TestVertex { float A; };

	const TestVertex verts1[] = { {1.0f}, {2.0f} };
	const std::uint16_t inds1[] = { 0, 1, 0 };

	const TestVertex verts2[] = { {3.0f}, {4.0f}, {5.0f} };
	const std::uint16_t inds2[] = { 0, 1, 2 };

	blResourceHandleOfType<blIndexedMeshResource> mesh1 = CreateIndexedMeshResource(
		container,
		L"Mesh1",
		descResource.GetRef(),
		verts1,
		(uint32_t)(sizeof(verts1) / sizeof(verts1[0])),
		inds1,
		(uint32_t)(sizeof(inds1) / sizeof(inds1[0]))
	);

	blResourceHandleOfType<blIndexedMeshResource> mesh2 = CreateIndexedMeshResource(
		container,
		L"Mesh2",
		descResource.GetRef(),
		verts2,
		(uint32_t)(sizeof(verts2) / sizeof(verts2[0])),
		inds2,
		(uint32_t)(sizeof(inds2) / sizeof(inds2[0]))
	);

	std::vector<blResourceRefOfType<blIndexedMeshResource>> refs;
	refs.push_back(mesh1.GetRef());
	refs.push_back(mesh2.GetRef());

	blResourceHandleOfType<blCompositeMeshResource_exp> compHandle =
		container->CreateResourceOfTypeWithDynamicSize<blCompositeMeshResource_exp>(L"CompositeMesh", refs);

	const blCompositeMeshResource_exp& comp = *compHandle;

	const auto* entry1 = comp.FindMeshEntry(mesh1.GetId());
	ASSERT_NE(entry1, nullptr);
	EXPECT_EQ(entry1->mVertexCount, (uint32_t)2);
	EXPECT_EQ(entry1->mIndexCount, (uint32_t)3);
	EXPECT_EQ(entry1->mVertexOffset, (uint64_t)0);
	EXPECT_EQ(entry1->mIndexOffset, (uint64_t)0);

	const auto* entry2 = comp.FindMeshEntry(mesh2.GetId());
	ASSERT_NE(entry2, nullptr);
	EXPECT_EQ(entry2->mVertexCount, (uint32_t)3);
	EXPECT_EQ(entry2->mIndexCount, (uint32_t)3);
	EXPECT_EQ(entry2->mVertexOffset, (uint64_t)2);
	// Note: blCompositeMeshResource_exp sets index offset to the previous mesh's index count
	EXPECT_EQ(entry2->mIndexOffset, (uint64_t)3);
}
