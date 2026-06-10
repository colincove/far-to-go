#pragma once

#include <Resources/blResource.h>
#include <blMeshIndexedCatalogue.h>
#include <blMesh.h>
#include <set>
#include <vector>
#include <map>

namespace BoulderLeaf::Graphics
{
	class blCompositeMesh
	{
	public:
		struct MeshEntry
		{
			blMeshBaseResourcePtr Mesh;
			int id;
		};
	private:
		blMeshIndexedCatalogue mCatalogue;
		std::map<int, blMeshIndexedCatalogue::index> mExternalIdToMeshInternalIdMap;
		std::map<blMeshIndexedCatalogue::index, MeshEntry> mInternalIdToMeshEntryMap;
	public:
		blCompositeMesh();
		blCompositeMesh(const std::vector<MeshEntry> meshData);

		const blMeshIndexedCatalogue& GetCatalogue() const { return mCatalogue; }

		const MeshEntry& GetMeshEntry(blMeshIndexedCatalogue::index index) const
		{
			return mInternalIdToMeshEntryMap.at(index);
		}
	};
}

BL_RESOURCE(blCompositeMesh, eResourceType::Mesh)