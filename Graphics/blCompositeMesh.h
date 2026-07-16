#pragma once

#include <blMesh.h>
#include <set>
#include <vector>
#include <map>

namespace BoulderLeaf
{
	template<typename T>
	class blResourceHandleOfType;
}

namespace BoulderLeaf::Graphics
{
	class CompositeMeshConstIterator;

	struct blCompositeMeshResource : public blListResource
	{
		struct MeshEntry
		{
			blResourceRefOfType<blIndexedMeshResource> mMeshResourceRef;

			uint64_t mVertexBufferSize;
			uint32_t mVertexCountOffset;
			uint32_t mVertexCount;
			uint64_t mIndexBufferSize;
			uint32_t mIndexCountOffset;
			uint32_t mIndexCount;
		};

		blCompositeMeshResource(
			blResourceStream& stream,
			const std::vector<blResourceRefOfType<blIndexedMeshResource>>& meshResourceRefs);

		const MeshEntry* FindMeshEntry(BoulderLeaf::blResourceId id) const;

		uint64_t VertexElementSize() const;
		uint64_t VertexBufferSize() const;
		uint64_t IndexBufferSize() const;
		uint32_t VertexCount() const;
		uint32_t IndexCount() const;
	};

	void GraftCompositeMeshToResource(
		const blCompositeMeshResource& compositeMesh,
		blResourceHandleOfType<blIndexedMeshResource> resource);

	blResourceHandleOfType<blIndexedMeshResource> CreateGraftedCompositeMeshResource(const blResourceHandleOfType<blCompositeMeshResource> compositeMesh);
}