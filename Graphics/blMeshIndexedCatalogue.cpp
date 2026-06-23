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
		const index newIndex = mCurrentIndex;
		const Entry& lastEntry = GetLastEntry();
		const blMeshStorage::Header header = mesh.GetHeader();

		size_t vertexBufferSize = header.GetVertexBufferSize();
		size_t indexBufferSize = header.GetIndexBufferSize();

		size_t vertexCountOffset = 0;
		size_t indexCountOffset = 0;

		for (const Entry& entry : mEntries)
		{
			vertexCountOffset += entry.VertexCount;
			indexCountOffset += entry.IndexCount;
		}

		mEntries.push_back(Entry(
			vertexBufferSize,
			//lastEntry.VertexOffset + lastEntry.VertexBufferSize, WHICH IS IT. BUGGG
			vertexCountOffset,
			header.mVertexCount,
			indexBufferSize,
			//lastEntry.IndexOffset + lastEntry.IndexBufferSize, // WHICH IS IT. BUGGGG
			indexCountOffset,
			header.mIndexCount
		));

		mCurrentIndex++;
		return newIndex;
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