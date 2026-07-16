#include <blCompositeMesh.h>

namespace BoulderLeaf::Graphics
{
	blCompositeMeshResource::blCompositeMeshResource(
		blResourceStream& stream,
		const std::vector<blResourceRefOfType<blIndexedMeshResource>>& meshResourceRefs)
		: blListResource(stream, (uint32_t) meshResourceRefs.size(), sizeof(MeshEntry))
	{
		uint32_t vertexCountOffset = 0;
		uint32_t indexCountOffset = 0;

		for (int i = 0; i < meshResourceRefs.size(); ++i)
		{
			const blIndexedMeshResource& meshResource = *stream.GetContainer()->CreateHandleFromRefOfType<blIndexedMeshResource>(meshResourceRefs[i]);
			const blListResource& indexBuffer = *stream.GetContainer()->CreateHandleFromRefOfType<blListResource>(meshResource.mIndexListRef);
			const blArrayBufferResource& vertexArrayBuffer = *stream.GetContainer()->CreateHandleFromRefOfType<blArrayBufferResource>(meshResource.mArrayBufferResourceRef);
			const blListResource& vertexBuffer = *stream.GetContainer()->CreateHandleFromRefOfType<blListResource>(vertexArrayBuffer.mBufferResourceRef);

			MeshEntry& entry = this->GetMutable<MeshEntry>(i);
			entry.mMeshResourceRef = meshResourceRefs[i];
			entry.mVertexBufferSize = vertexBuffer.mCount * vertexBuffer.mElementSize;
			entry.mVertexCountOffset = vertexCountOffset;
			entry.mVertexCount = vertexBuffer.mCount;
			entry.mIndexBufferSize = indexBuffer.mCount * indexBuffer.mElementSize;
			entry.mIndexCountOffset = indexCountOffset;
			entry.mIndexCount = indexBuffer.mCount;

			vertexCountOffset += vertexBuffer.mCount;
			indexCountOffset += indexBuffer.mCount;
		}
	}

	const blCompositeMeshResource::MeshEntry* blCompositeMeshResource::FindMeshEntry(BoulderLeaf::blResourceId id) const
	{
		for (int i = 0; i < mCount; ++i)
		{
			const MeshEntry& entry = this->Get<MeshEntry>(i);
			if (entry.mMeshResourceRef.GetId() == id)
			{
				return &entry;
			}
		}
		assert(false);
		return nullptr;
	}

	uint64_t blCompositeMeshResource::VertexElementSize() const
	{
		const MeshEntry& firstEntry = Get<MeshEntry>(0);
		return firstEntry.mVertexBufferSize / firstEntry.mVertexCount;
	}

	uint64_t blCompositeMeshResource::VertexBufferSize() const
	{
		uint64_t totalSize = 0;

		for (int i = 0; i < mCount; ++i)
		{
			totalSize += Get<MeshEntry>(i).mVertexBufferSize;
		}

		return totalSize;
	}

	uint64_t blCompositeMeshResource::IndexBufferSize() const
	{
		uint64_t totalSize = 0;
		for (int i = 0; i < mCount; ++i)
		{
			totalSize += Get<MeshEntry>(i).mIndexBufferSize;
		}
		return totalSize;
	}

	uint32_t blCompositeMeshResource::VertexCount() const
	{
		uint64_t totalCount = 0;

		for (int i = 0; i < mCount; ++i)
		{
			totalCount += Get<MeshEntry>(i).mVertexCount;
		}

		return totalCount;
	}

	uint32_t blCompositeMeshResource::IndexCount() const
	{
		uint64_t totalCount = 0;

		for (int i = 0; i < mCount; ++i)
		{
			totalCount += Get<MeshEntry>(i).mIndexCount;
		}

		return totalCount;
	}

	void GraftCompositeMeshToResource(
		const blCompositeMeshResource& compositeMesh,
		blResourceHandleOfType<blIndexedMeshResource> destinationMeshResource)
	{
		blResourceContainer* container = destinationMeshResource.GetContainer();

		for (int i = 0; i < compositeMesh.mCount; ++i)
		{
			const blCompositeMeshResource::MeshEntry& meshEntry = compositeMesh.Get<blCompositeMeshResource::MeshEntry>(i);
			
			blIndexedMeshResource& destinationMesh = *destinationMeshResource;
			blListResource& destinationIndexBuffer =
				*container->CreateHandleFromRefOfType<blListResource>(destinationMesh.mIndexListRef);
			blArrayBufferResource& destinationVertexArrayBuffer =
				*container->CreateHandleFromRefOfType<blArrayBufferResource>(destinationMesh.mArrayBufferResourceRef);
			blListResource& destinationVertexBuffer =
				*container->CreateHandleFromRefOfType<blListResource>(destinationVertexArrayBuffer.mBufferResourceRef);

			blIndexedMeshResource& sourceMeshResource =
				*container->CreateHandleFromRefOfType<blIndexedMeshResource>(meshEntry.mMeshResourceRef);
			blListResource& sourceIndexBuffer =
				*container->CreateHandleFromRefOfType<blListResource>(sourceMeshResource.mIndexListRef);
			blArrayBufferResource& sourceVertexArrayBuffer =
				*container->CreateHandleFromRefOfType<blArrayBufferResource>(sourceMeshResource.mArrayBufferResourceRef);
			blListResource& sourceVertexBuffer =
				*container->CreateHandleFromRefOfType<blListResource>(sourceVertexArrayBuffer.mBufferResourceRef);

			memcpy(&destinationIndexBuffer.GetMutable<byte>(meshEntry.mIndexCountOffset), &sourceIndexBuffer.Get<byte>(0), meshEntry.mIndexBufferSize);
			memcpy(&destinationVertexBuffer.GetMutable<byte>(meshEntry.mVertexCountOffset), &sourceVertexBuffer.Get<byte>(0), meshEntry.mVertexBufferSize);
		}
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateGraftedCompositeMeshResource(const blResourceHandleOfType<blCompositeMeshResource> compositeMeshHandle)
	{
		blResourceContainer* container = compositeMeshHandle.GetContainer();
		const blCompositeMeshResource::MeshEntry& firstEntry = compositeMeshHandle->Get<blCompositeMeshResource::MeshEntry>(0);
		blResourceHandleOfType<blIndexedMeshResource> firstEntryMeshResource = container->CreateHandleFromRefOfType<blIndexedMeshResource>(firstEntry.mMeshResourceRef);
		blResourceHandleOfType<blArrayBufferResource> firstEntryVertexBufferResource = container->CreateHandleFromRefOfType<blArrayBufferResource>(firstEntryMeshResource->mArrayBufferResourceRef);

		blResourceHandleOfType<blIndexedMeshResource> graftedMeshResource = CreateIndexedMeshResource(
			container,
			std::wstring(container->GetDataName(compositeMeshHandle.GetId())) + L"Grafted",
			firstEntryVertexBufferResource->mDescriptionResourceRef,
			compositeMeshHandle->VertexElementSize(),
			compositeMeshHandle->VertexCount(),
			compositeMeshHandle->IndexCount()
		);

		GraftCompositeMeshToResource(*compositeMeshHandle, graftedMeshResource);

		return graftedMeshResource;
	}
}