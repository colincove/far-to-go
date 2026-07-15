#include <blCompositeMesh.h>
#include <cstdarg>

namespace BoulderLeaf::Graphics
{
	blCompositeMesh::blCompositeMesh(size_t count, ...)
		: mExternalIdToMeshInternalIdMap(),
		mInternalIdToMeshEntryMap(),
		mCatalogue()
	{
		std::va_list args;
		va_start(args, count);

		for (size_t i = 0; i < count; ++i) 
		{
			blMeshBaseResourcePtr meshPtr = va_arg(args, blMeshBaseResourcePtr);
			
			const blMeshIndexedCatalogue::index internalId = mCatalogue.AddMesh(meshPtr->GetData().GetStorage());
			mInternalIdToMeshEntryMap[internalId] = meshPtr;
			mExternalIdToMeshInternalIdMap[meshPtr->GetId()] = internalId;
		}

		va_end(args);
	}

	size_t blCompositeMesh::GetIndexBufferSize() const
	{
		size_t total = 0;

		for (const blMeshBaseResourcePtr& mesh : CompositeMeshConstIterator(this))
		{
			const blMeshStorage::Header& storageHeader = mesh->GetData().GetStorage().GetHeader();
			total += storageHeader.GetIndexBufferSize();
		}

		return total;
	}

	size_t blCompositeMesh::GetVertexBufferSize() const
	{
		size_t total = 0;

		for (const blMeshBaseResourcePtr& mesh : CompositeMeshConstIterator(this))
		{
			const blMeshStorage::Header& storageHeader = mesh->GetData().GetStorage().GetHeader();
			total += storageHeader.GetVertexBufferSize();
		}

		return total;
	}

	blCompositeMeshResource_exp::blCompositeMeshResource_exp(
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
			entry.mVertexOffset = vertexCountOffset;
			entry.mVertexCount = vertexBuffer.mCount;
			entry.mIndexBufferSize = indexBuffer.mCount * indexBuffer.mElementSize;
			entry.mIndexOffset = indexCountOffset;
			entry.mIndexCount = indexBuffer.mCount;

			vertexCountOffset += vertexBuffer.mCount;
			indexCountOffset = indexBuffer.mCount;
		}
	}

	const blCompositeMeshResource_exp::MeshEntry* blCompositeMeshResource_exp::FindMeshEntry(BoulderLeaf::blResourceId id) const
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
}