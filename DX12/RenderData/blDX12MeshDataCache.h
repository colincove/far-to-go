#pragma once

#include <vector>
#include <memory>
#include <blDevice.h>
#include <blDX12ResourceDataCache.h>
#include <blMesh.h>


namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12MeshStorageCacheData
	{
		blMeshStorage meshStorage;
	};

	class blDX12MeshStorageCache : public blDX12ResourceDataCache<blDX12MeshStorageCacheData, blMeshBaseResource>
	{
	protected:
		using super = blDX12ResourceDataCache<blDX12MeshStorageCacheData, blMeshBaseResource>;
	private:
		std::shared_ptr<blDevice> mDevice;
	public:
		blDX12MeshStorageCache(std::shared_ptr<blDevice> device);
		virtual void InitializeCache(
			const blMeshBaseResource& resource,
			blDX12MeshStorageCacheData& cache) override;
	};
}