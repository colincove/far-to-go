#include <blCompositeMeshRenderWithPassConstantsRenderComponent.h>

#include <RenderData/blDX12CompositeMeshDataCache.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshRenderWithPassConstantsRenderComponent::blCompositeMeshRenderWithPassConstantsRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr) :
		blRenderComponent(globalRenderDataPtr),
		mCompositeMeshStorageCache(std::make_shared<blCompositeMeshDataCache>(
			globalRenderDataPtr->device,
			mCommandList,
			globalRenderDataPtr->meshStorageCache)),
		mMeshDataDeviceCache(std::make_shared<blDX12MeshDataDeviceCache>(
			globalRenderDataPtr->device,
			mCommandList,
			globalRenderDataPtr->meshStorageCache))
	{
	}

	void blCompositeMeshRenderWithPassConstantsRenderComponent::Render(const RenderCompositeMeshDataWithPassConstants& renderData, const blSceneResourcePtr scene)
	{
		if (!renderData.compositeMesh)
		{
			return;
		}

		blGlobalRenderData& globalRenderData = *mGlobalRenderData.get();

		const blCompositeMeshDataCacheData& resourceCache = mCompositeMeshStorageCache->Get(
			*renderData.compositeMesh
		);

		const blDX12MeshDataDeviceCacheData& meshDeviceStorageCacheData = mMeshDataDeviceCache->Get(*resourceCache.meshResource);

		const blMaterial& material = renderData.material->GetData();
		const blShaderResource& shaderResource = *material.shader;
		const blShader& shader = shaderResource.GetData();
		const blShaderCacheData& shaderCacheData = globalRenderData.shaderCache->Get(shaderResource);
		const blPSOCacheData& psoCacheData = globalRenderData.mPSOCache->Get(shaderResource);

		// Do I not do anything with this data? How do constant buffers get bound? I'm confused. 
		//TODO: Why is this data specific to standard object constants? I want to be able to use this for any type of constant buffer.
		const blDX12ConstantBufferCacheData& constantBufferCache = globalRenderData.constantBufferCache->Get(*renderData.constantBuffer.get());

		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = globalRenderData.depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = globalRenderData.swapChain->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = meshDeviceStorageCacheData.meshGeometry.VertexBufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = meshDeviceStorageCacheData.meshGeometry.IndexBufferView();

		// Specify the buffers we are going to render to.
		mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);
		ID3D12DescriptorHeap* descriptorHeaps[] = { constantBufferCache.descriptorHeap->GetDescriptorHeap().Get() };
		mCommandList->SetDescriptorHeaps((UINT)_countof(descriptorHeaps), descriptorHeaps);
		mCommandList->SetGraphicsRootSignature(shaderCacheData.RootSignature->GetRootSignature().Get());
		mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		mCommandList->IASetIndexBuffer(&indexBufferView);
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetPipelineState(psoCacheData.PSO->GetDX12PSO().Get());
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferCache.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());

		const size_t count = renderData.constantBuffer->GetData().Count();
		cbvHandle.Offset((UINT)((globalRenderData.device->GetCbvSrvDescriptorSize() * count) * globalRenderData.globalRenderFrameContext->GetCurrentFrameResource()));

		const size_t vertexSize = resourceCache.meshResource->GetData().GetVertexSize();

		for (int i = 0; i < count; ++i)
		{
			blMeshIndexedCatalogue::Entry entry = renderData.compositeMesh->GetData().GetMeshEntry(renderData.submeshId);
			mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

			mCommandList->DrawIndexedInstanced(
				(UINT)entry.IndexCount,
				1,
				(UINT)entry.IndexOffset,
				(UINT)entry.VertexOffset,
				0);

			cbvHandle.Offset(globalRenderData.device->GetCbvSrvDescriptorSize());
		}
	}
}