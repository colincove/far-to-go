#pragma once

#include <vector>
#include <memory>
#include <blDevice.h>
#include <blDX12ResourceDataCache.h>
#include <blDX12MeshDataCache.h>
#include <blCommandList.h>
#include <blDX12MeshGeometry.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12MeshDataDeviceCacheData
	{
		blDX12Mesh meshGeometry;
		SubmeshGeometry submesh;
	};

	class blDX12MeshDataDeviceCache : public blDX12ResourceDataCache<blDX12MeshDataDeviceCacheData, blMeshBaseResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blCommandList> mCommandList;
		std::shared_ptr<blDX12MeshStorageCache> mMeshStorageCache;
	public:
		blDX12MeshDataDeviceCache(
			std::shared_ptr<blDevice> device, 
			std::shared_ptr<blCommandList> mCommandList,
			std::shared_ptr<blDX12MeshStorageCache> meshStorageCache
			);

		virtual void InitializeCache(
			const blMeshBaseResource& resource,
			blDX12MeshDataDeviceCacheData& cache) override;
	};
}