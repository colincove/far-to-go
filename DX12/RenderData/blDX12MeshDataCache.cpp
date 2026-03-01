#include <blDX12MeshDataCache.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12MeshStorageCache::blDX12MeshStorageCache(std::shared_ptr<blDevice> device)
		: mDevice(device), blDX12ResourceDataCache()
	{

	}

	void blDX12MeshStorageCache::InitializeCache(
		const blMeshBaseResource& resource,
		blDX12MeshStorageCacheData& cache)
	{
		using namespace DirectX;
		const blMeshBaseResource::DataType& meshData = resource.GetData();
		const blMeshStorage& meshStorage = meshData.GetStorage();

		const size_t vertexSize = GetBufferElementSize(
			meshData.GetVertexElementDescriptions(),
			DX12::DX12BufferAdapter::Get());

		cache.meshStorage = std::move(blMeshStorage(
			meshStorage.GetHeader().mVertexCount,
			meshStorage.GetHeader().mIndexCount,
			vertexSize));

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