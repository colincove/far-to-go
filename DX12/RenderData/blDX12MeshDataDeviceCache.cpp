#include <blDX12MeshDataDeviceCache.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12VertexBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12MeshDataDeviceCache::blDX12MeshDataDeviceCache(
		blDevice* device,
		blCommandList* commandList,
		blDX12MeshStorageCache* meshStorageCache)
		: super(), mDevice(device), mCommandList(commandList), mMeshStorageCache(meshStorageCache)
	{

	}

	void blDX12MeshDataDeviceCache::InitializeCache(
		const blMeshBaseResource& resource,
		blDX12MeshDataDeviceCacheData& cache)
	{
		using namespace DirectX;

		const blDX12MeshStorageCacheData& meshStorageCacheData = mMeshStorageCache->Get(resource);

		GenerateSubresources(
			meshStorageCacheData.meshStorage,
			mDevice->GetDX12Device().Get(),
			mCommandList->GetCommandListPtr().Get(),
			cache.meshGeometry,
			cache.submesh);

		cache.meshGeometry.SetResourceNames(resource.GetName());
	}
}