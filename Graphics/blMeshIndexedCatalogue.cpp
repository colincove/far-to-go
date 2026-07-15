#include <blMeshIndexedCatalogue.h>
#include <assert.h>

namespace BoulderLeaf::Graphics
{
	blMeshIndexedCatalogue::blMeshIndexedCatalogue() :
		mEntries(),
		mCurrentIndex(0) 
	{}

	const blMeshIndexedCatalogue::Entry& blMeshIndexedCatalogue::GetLastEntry() const
	{
		if (mEntries.empty())
		{
			static Entry sDefaultEntry;
			return sDefaultEntry; // Return a default entry if no entries exist
		}

		return mEntries[mCurrentIndex - 1];
	}

	blMeshIndexedCatalogue::index blMeshIndexedCatalogue::AddMesh(const blMeshStorage& mesh)
	{
		const blMeshStorage::Header header = mesh.GetHeader();
		return AddMesh(header.mVertexCount, header.mVertexSize, header.mIndexCount);
	}

	blMeshIndexedCatalogue::index blMeshIndexedCatalogue::AddMesh(
		uint32_t vertexCount,
		uint64_t vertexSize,
		uint32_t indexCount)
	{
		const index newIndex = mCurrentIndex;
		const Entry& lastEntry = GetLastEntry();

		size_t vertexBufferSize = vertexCount * vertexSize;
		size_t indexBufferSize = indexCount * sizeof(uint16_t);

		size_t vertexCountOffset = 0;
		size_t indexCountOffset = 0;

		for (const Entry& entry : mEntries)
		{
			vertexCountOffset += entry.VertexCount;
			indexCountOffset += entry.IndexCount;
		}

		mEntries.push_back(Entry(
			vertexBufferSize,
			vertexCountOffset,
			vertexCount,
			indexBufferSize,
			indexCountOffset,
			indexCount
		));

		mCurrentIndex++;
		return newIndex;
	}

	blMeshIndexedCatalogue::index blMeshIndexedCatalogue::AddMeshResource(const blResourceHandleOfType<blIndexedMeshResource> meshHandle)
	{
		blResourceContainer* container = meshHandle.GetContainer();

		const blResourceHandleOfType<blListResource> indexBuffer = container->CreateHandleFromRefOfType<blListResource>(
			meshHandle->mIndexListRef
		);

		const blResourceHandleOfType<blArrayBufferResource> vertexBuffer = container->CreateHandleFromRefOfType<blArrayBufferResource>(
			meshHandle->mArrayBufferResourceRef
		);

		const blResourceHandleOfType<blListResource> vertexList = container->CreateHandleFromRefOfType<blListResource>(
			vertexBuffer->mBufferResourceRef
		);

		const blResourceHandleOfType<blBufferDescriptionResource> description = container->CreateHandleFromRefOfType<blBufferDescriptionResource>(
			vertexBuffer->mDescriptionResourceRef
		);

		return AddMesh(vertexList->mCount, vertexList->mElementSize, indexBuffer->mCount);
	}

	const blMeshIndexedCatalogue::Entry& blMeshIndexedCatalogue::GetEntry(index idx) const
	{
		assert(idx < mEntries.size() && "Index out of bounds");
		return mEntries[idx];
	}

	blMeshIndexedCatalogue::Entry& blMeshIndexedCatalogue::GetEntryMutable(index idx)
	{
		assert(idx < mEntries.size() && "Index out of bounds");
		return mEntries[idx];
	}
}