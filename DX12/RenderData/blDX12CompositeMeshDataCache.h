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
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blCommandList> mCommandList;
		std::shared_ptr<blDX12MeshStorageCache> mMeshStorageCache;
	public:
		blCompositeMeshDataCache(
			std::shared_ptr<blDevice> device,
			std::shared_ptr<blCommandList> commandList,
			std::shared_ptr<blDX12MeshStorageCache> meshStorageCache);

		virtual void InitializeCache(
			const blCompositeMeshResource& resource,
			blCompositeMeshDataCacheData& cache) override;
	};
}