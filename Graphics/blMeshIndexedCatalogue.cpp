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
		mCurrentIndex++;

		const Entry& lastEntry = GetLastEntry();

		mEntries.push_back(Entry(
			lastEntry.VertexOffset + lastEntry.VertexBufferSize,
			mesh.GetHeader().GetVertexBufferSize(),
			lastEntry.IndexOffset + lastEntry.IndexBufferSize,
			mesh.GetHeader().GetIndexBufferSize()
		));

		return newIndex;
	}

	blMeshIndexedCatalogue::Entry& blMeshIndexedCatalogue::GetEntry(index idx)
	{
		assert(idx < mEntries.size() && "Index out of bounds");
		return mEntries[idx];
	}
}