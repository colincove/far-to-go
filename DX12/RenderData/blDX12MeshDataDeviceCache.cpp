#include <blDX12MeshDataDeviceCache.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12VertexBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12MeshDataDeviceCache::blDX12MeshDataDeviceCache(
		std::shared_ptr<blDevice> device,
		std::shared_ptr<blCommandList> commandList,
		std::shared_ptr<blDX12MeshStorageCache> meshStorageCache)
		: mDevice(device), mCommandList(commandList), mMeshStorageCache(meshStorageCache),blDX12ResourceDataCache()
	{

	}

	void blDX12MeshDataDeviceCache::InitializeCache(
		const blMeshBaseResource& resource,
		blDX12MeshDataDeviceCacheData& cache)
	{
		using namespace DirectX;

		const blDX12MeshStorageCacheData& meshStorageCacheData = mMeshStorageCache->Get(resource);
		const blMeshStorage& storage = meshStorageCacheData.meshStorage;
		const blMeshStorage::Header& header = storage.GetHeader();
		blDX12Mesh& meshGeometry = cache.meshGeometry;
		
		D3DCreateBlob(header.GetVertexBufferSize(), &meshGeometry.VertexBufferCPU);
		CopyMemory(meshGeometry.VertexBufferCPU->GetBufferPointer(), storage.VertexBegin(), header.GetVertexBufferSize());
		D3DCreateBlob(header.GetVertexBufferSize(), &meshGeometry.IndexBufferCPU);
		CopyMemory(meshGeometry.IndexBufferCPU->GetBufferPointer(), storage.IndexBegin(), header.GetVertexBufferSize());

		meshGeometry.VertexBufferGPU = CreateDefaultBuffer(
			mDevice->GetDX12Device().Get(),
			mCommandList->GetCommandListPtr().Get(),
			storage.VertexBegin(),
			header.GetVertexBufferSize(),
			meshGeometry.VertexBufferUploader);

		meshGeometry.IndexBufferGPU = CreateDefaultBuffer(
			mDevice->GetDX12Device().Get(),
			mCommandList->GetCommandListPtr().Get(),
			storage.IndexBegin(),
			header.GetVertexBufferSize(),
			meshGeometry.IndexBufferUploader);

		meshGeometry.VertexByteStride = (UINT) header.mVertexSize;
		meshGeometry.VertexBufferByteSize = (UINT) header.GetVertexBufferSize();
		meshGeometry.IndexFormat = DXGI_FORMAT_R16_UINT;
		meshGeometry.IndexBufferByteSize = (UINT) header.GetVertexBufferSize();
		
		SubmeshGeometry submesh;
		submesh.IndexCount = (UINT) storage.GetIndexCount();
		submesh.StartIndexLocation = 0;
		submesh.BaseVertexLocation = 0;
		cache.submesh = submesh;
	}
}