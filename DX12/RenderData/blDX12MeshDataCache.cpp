#include <blDX12MeshDataCache.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12MeshStorageCache::blDX12MeshStorageCache(std::shared_ptr<blDevice> device)
		: mDevice(device), super()
	{

	}

	void blDX12MeshStorageCache::InitializeCache(
		const blMeshBaseResource& resource,
		blDX12MeshStorageCacheData& cache)
	{
		using namespace DirectX;
		const blMeshBaseResource::DataType& meshData = resource.GetData();
		const blMeshStorage& meshStorage = meshData.GetStorage();

		cache.meshStorage =
			std::move(blMeshStorage(
			meshStorage.GetHeader().mVertexCount,
			meshStorage.GetHeader().mIndexCount,
			meshStorage.GetVertexSize(),
			DX12::DX12BufferAdapter::Get().GetFormat()));

		memcpy(static_cast<void*>(cache.meshStorage.IndexBeginMutable()), static_cast<const void*>(meshStorage.IndexBegin()), meshStorage.GetIndexCount() * sizeof(blMeshStorage::index));

		MarshalBuffer(
			meshStorage.GetHeader().mVertexCount,
			meshData.GetVertexElementDescriptions(),
			[&meshStorage](size_t index)
			{
				return static_cast<const byte*>(meshStorage.GetVertex(index));
			},
			[&cache](size_t index)
			{
				return static_cast<byte*>(cache.meshStorage.GetVertexMutable(index));
			},
			DX12::DX12BufferAdapter::Get()
		);
	}
}