#pragma once

#include <blDX12ResourceDataCache.h>
#include <blCompositeMesh.h>
#include <blDevice.h>
#include <blDX12MeshDataCache.h>
#include <blCommandList.h>
#include <blDX12MeshGeometry.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blCompositeMeshDataCacheData
	{
		blMeshBaseResourcePtr meshResource;
	};

	class blCompositeMeshDataCache : public blDX12ResourceDataCache<blCompositeMeshDataCacheData, blCompositeMeshResource>
	{
	private:
		blDevice* mDevice;
		blCommandList* mCommandList;
		blDX12MeshStorageCache* mMeshStorageCache;
	public:
		blCompositeMeshDataCache(
			blDevice* device,
			blCommandList* commandList,
			blDX12MeshStorageCache* meshStorageCache);

		virtual void InitializeCache(
			const blCompositeMeshResource& resource,
			blCompositeMeshDataCacheData& cache) override;
	};
}