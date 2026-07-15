#pragma once

#include <Resources/blResource.h>
#include <blMeshIndexedCatalogue.h>
#include <blMesh.h>
#include <set>
#include <vector>
#include <map>

namespace BoulderLeaf::Graphics
{
	class CompositeMeshConstIterator;

	class blCompositeMesh
	{
		friend class CompositeMeshConstIterator;
	private:

		blMeshIndexedCatalogue mCatalogue;
		std::map<blResourceId, blMeshIndexedCatalogue::index> mExternalIdToMeshInternalIdMap;
		std::map<blMeshIndexedCatalogue::index, blMeshBaseResourcePtr> mInternalIdToMeshEntryMap;
	public:

		// this was fun and interesting to write. But it is error prone. Revisit in the future. 
		// variadic templates sound safer? But this class is not templated. So can I have a variadic template constructor? Maybe not.
		blCompositeMesh(size_t count, ...);

		const blMeshIndexedCatalogue& GetCatalogue() const { return mCatalogue; }

		blMeshIndexedCatalogue::Entry GetMeshEntry(blResourceId id) const
		{
			if (auto entry = mExternalIdToMeshInternalIdMap.find(id); entry != mExternalIdToMeshInternalIdMap.end())
			{
				return mCatalogue.GetEntry(entry->second);
			}

			assert(false);
			return blMeshIndexedCatalogue::Entry();
		}

		size_t GetIndexBufferSize() const;
		size_t GetVertexBufferSize() const;
		BufferFormat GetFormat() const { return mInternalIdToMeshEntryMap.begin()->second->GetData().GetFormat(); }

	};

	struct blCompositeMeshResource_exp : public blListResource
	{
		struct MeshEntry
		{
			blResourceRefOfType<blIndexedMeshResource> mMeshResourceRef;

			uint64_t mVertexBufferSize;
			uint64_t mVertexOffset;
			uint32_t mVertexCount;
			uint64_t mIndexBufferSize;
			uint64_t mIndexOffset;
			uint32_t mIndexCount;
		};

		blCompositeMeshResource_exp(
			blResourceStream& stream,
			const std::vector<blResourceRefOfType<blIndexedMeshResource>>& meshResourceRefs);

		const MeshEntry* FindMeshEntry(BoulderLeaf::blResourceId id) const;
	};
}

BL_RESOURCE(blCompositeMesh, eResourceType::Mesh)

namespace BoulderLeaf::Graphics
{
	class CompositeMeshConstIterator
	{
	private:
		const blCompositeMesh* mCompositeMesh;
		blMeshIndexedCatalogue::index mIndex;

		CompositeMeshConstIterator(const blCompositeMesh* compositeMesh, blMeshIndexedCatalogue::index index) :
			mCompositeMesh(compositeMesh),
			mIndex(index) {
		}
	public:
		CompositeMeshConstIterator(const blCompositeMesh* compositeMesh) :
			mCompositeMesh(compositeMesh),
			mIndex(0) {}

		const blMeshBaseResourcePtr operator*() const
		{
			return mCompositeMesh->mInternalIdToMeshEntryMap.at(mIndex);
		}

		CompositeMeshConstIterator& operator++()
		{
			++mIndex;
			return *this;
		}

		bool operator!=(const CompositeMeshConstIterator& other) const
		{
			return mIndex != other.mIndex;
		}

		CompositeMeshConstIterator begin() const { return CompositeMeshConstIterator(mCompositeMesh); }
		CompositeMeshConstIterator end() const { return CompositeMeshConstIterator(mCompositeMesh, mCompositeMesh->mCatalogue.GetMeshCount()); }
	};
};