#include <blCompositeMesh.h>

namespace BoulderLeaf::Graphics
{
	blCompositeMesh::blCompositeMesh() : blCompositeMesh(std::vector<MeshEntry>()){}

	blCompositeMesh::blCompositeMesh(const std::vector<MeshEntry> meshData)
		: mExternalIdToMeshInternalIdMap(),
		mInternalIdToMeshEntryMap(),
		mCatalogue()
	{
		for (const MeshEntry& entry : meshData)
		{
			const blMeshIndexedCatalogue::index internalId = mCatalogue.AddMesh(entry.Mesh->GetData().GetStorage());
			mInternalIdToMeshEntryMap[internalId] = entry;
			mExternalIdToMeshInternalIdMap[entry.id] = internalId;
		}
	}
}