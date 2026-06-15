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

		const blMeshBaseResourcePtr& GetMeshEntry(blMeshIndexedCatalogue::index index) const
		{
			return mInternalIdToMeshEntryMap.at(index);
		}

		size_t GetIndexBufferSize() const;
		size_t GetVertexBufferSize() const;
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