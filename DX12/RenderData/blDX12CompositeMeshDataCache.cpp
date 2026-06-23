#include <RenderData/blDX12CompositeMeshDataCache.h>

#include <format>
#include <DirectXMath.h>
#include <blDX12VertexBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshDataCache::blCompositeMeshDataCache(
		std::shared_ptr<blDevice> device,
		std::shared_ptr<blCommandList> commandList,
		std::shared_ptr<blDX12MeshStorageCache> meshStorageCache)
		: mDevice(device), 
		mCommandList(commandList),  
		mMeshStorageCache(meshStorageCache),
		blDX12ResourceDataCache<blCompositeMeshDataCacheData, blCompositeMeshResource>()
	{

	}

	void blCompositeMeshDataCache::InitializeCache(
		const blCompositeMeshResource& resource,
		blCompositeMeshDataCacheData& cache)
	{
		using namespace DirectX;

		size_t indexCount = 0;
		size_t vertexCount = 0;
		size_t vertexSize = 0;
		std::vector<VertexElementDescription> vertexElementDescriptions;

		for (const blMeshBaseResourcePtr& meshEntry : CompositeMeshConstIterator(&resource.GetData()))
		{
			const blMeshStorage::Header& meshStorageHeader = meshEntry->GetData().GetStorage().GetHeader();
			vertexCount += meshStorageHeader.mVertexCount;
			indexCount += meshStorageHeader.mIndexCount;

			//right now, blCompositeMesh does not enforce the same vertex type. We should consider enforcing this in the future, but for now we will just assume that all meshes have the same vertex type and size as the first mesh.
			vertexSize = meshStorageHeader.mVertexSize;

			//this is dumb. I am lazy right now. 
			vertexElementDescriptions = meshEntry->GetData().GetVertexElementDescriptions();
		}

		// I tried to upload resources seperately. But I need to understand this better. 
		// For now, graft all of the meshes into one big vertex and index buffer and upload that.
		// And if I need this. For sure. Then I can probably stop treating it as a special type of mesh. 
		// Treat it as a normal mesh resource that can use all of the same render and caching paths.
		cache.meshResource = blResourceManager::Get().CreateResourceWithName<blMeshBaseResource>(
			resource.GetName(),
			vertexCount, 
			indexCount, 
			vertexSize, 
			resource.GetData().GetFormat());

		//this is dumb. I am laxy
		cache.meshResource->GetDataMutable().VertexElementDescriptions = vertexElementDescriptions;

		indexCount = 0;
		vertexCount = 0;
		for (const blMeshBaseResourcePtr& meshEntry : CompositeMeshConstIterator(&resource.GetData()))
		{
			const blMeshStorage& meshStorage = meshEntry->GetData().GetStorage();
			cache.meshResource->GetDataMutable().GetStorageMutable().GraftMeshData(meshStorage, indexCount, vertexCount, false);
			indexCount += meshStorage.GetIndexCount();
			vertexCount += meshStorage.GetVertexCount();
		}
	}
}