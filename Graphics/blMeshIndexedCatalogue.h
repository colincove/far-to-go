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
				: VertexBufferSize(0), VertexOffset(0), IndexBufferSize(0), IndexOffset(0) {
			}

			Entry(size_t vertexBufferSize, size_t vertexOffset, size_t indexBufferSize, size_t indexOffset)
				: VertexBufferSize(vertexBufferSize), VertexOffset(vertexOffset), IndexBufferSize(indexBufferSize), IndexOffset(indexOffset) {
			}

			size_t VertexBufferSize;
			size_t VertexOffset;
			size_t IndexBufferSize;
			size_t IndexOffset;
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
		Entry& GetEntry(index idx);
	};
}