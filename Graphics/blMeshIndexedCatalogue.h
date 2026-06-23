#pragma once

#include <blMesh.h>
#include <vector>

namespace BoulderLeaf::Graphics
{
	class blMeshIndexedCatalogue
	{
	public:
		using index = unsigned int;

		struct Entry
		{
			constexpr Entry() noexcept
				: VertexBufferSize(0), 
				VertexOffset(0), 
				VertexCount(0),
				IndexBufferSize(0), 
				IndexOffset(0),
				IndexCount(0)
			{
			}

			Entry(
				size_t vertexBufferSize, 
				size_t vertexOffset, 
				size_t vertexCount,
				size_t indexBufferSize, 
				size_t indexOffset,
				size_t indexCount)
				: VertexBufferSize(vertexBufferSize), 
				VertexOffset(vertexOffset), 
				VertexCount(vertexCount),
				IndexBufferSize(indexBufferSize), 
				IndexOffset(indexOffset) ,
				IndexCount(indexCount)
			{
			}

			size_t VertexBufferSize;
			size_t VertexOffset;
			size_t VertexCount;
			size_t IndexBufferSize;
			size_t IndexOffset;
			size_t IndexCount;
		};
	private:
		index mCurrentIndex;
		std::vector<Entry> mEntries;
	public:
		blMeshIndexedCatalogue();
	private:
		const Entry& GetLastEntry() const;
	public:
		index AddMesh(const blMeshStorage& mesh);
		const Entry& GetEntry(index idx) const;
		Entry& GetEntryMutable(index idx);
		index GetMeshCount() const { return mCurrentIndex; }
	};
}