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
}