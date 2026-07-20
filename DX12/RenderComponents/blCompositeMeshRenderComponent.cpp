#include <blCompositeMeshRenderComponent.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>
#include <blDX12Buffer.h>
#include <blDX12MeshGeometry.h>


namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshRenderComponent::blCompositeMeshRenderComponent(blGlobalRenderData* globalRenderDataPtr) :
		blRenderComponent(globalRenderDataPtr),
		mBufferUploadCache(globalRenderDataPtr->device.get(), mCommandList.get())
	{
	}

	void blCompositeMeshRenderComponent::Render(const RenderCompositeMeshDataInstanced& renderData)
	{
		blGlobalRenderData& globalRenderData = *mGlobalRenderData;
		blResourceContainer* container = renderData.compositeMesh.GetContainer();

		const blDX12CompositeMeshGraftingCacheData& graftedMeshCache =
			globalRenderData.resourceCacheGlobalInterface->GetCompositeMeshGraftingCache(renderData.compositeMesh);
		blResourceHandleOfType<blArrayBufferResource> graftedVertexBufferHandle =
			container->CreateHandleFromRefOfType<blArrayBufferResource>(graftedMeshCache.graftedMesh->mArrayBufferResourceRef);

		const blDX12ArrayBufferTranslationCacheData& translatedMeshCache =
			globalRenderData.resourceCacheGlobalInterface->GetArrayBufferTranslationCacheData(graftedVertexBufferHandle);
		const blResourceHandleOfType<blListResource>& translatedVertexListResource = container->CreateHandleFromRefOfType<blListResource>(
			translatedMeshCache.translatedArrayBuffer->mBufferResourceRef);
		const blResourceHandleOfType<blListResource>& indexListResource = container->CreateHandleFromRefOfType<blListResource>(
			graftedMeshCache.graftedMesh->mIndexListRef);

		const blMaterialResource& material = *renderData.material;
		const blResourceHandleOfType<blShaderResource> shaderResourceHandle =
			renderData.material.GetContainer()->CreateHandleFromRefOfType<blShaderResource>(material.mShaderResourceRef);

		const blDX12ShaderCacheData& shaderCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetShaderCacheData(shaderResourceHandle);

		const blDX12UploadBufferCacheData& vertexUploadCacheData =
			mBufferUploadCache.GetTypedCachedData(translatedVertexListResource);
		const blDX12UploadBufferCacheData& indexUploadCacheData =
			mBufferUploadCache.GetTypedCachedData(indexListResource);

		const blResourceHandleOfType<blListResource> constantBufferListResource = container->CreateHandleFromRefOfType<blListResource>(
			renderData.constantBuffer->mBufferResourceRef);
		const blDX12MappedUploadBufferCacheData& constantBufferUploadBufferCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetMappedUploadBufferCache(renderData.constantBuffer);

		const blResourceHandleOfType<blListResource> constantBufferResourceHandle =
			renderData.constantBuffer.GetContainer()->CreateHandleFromRefOfType<blListResource>(renderData.constantBuffer->mBufferResourceRef);
		const blDX12DescriptorHeapCacheData& descriptorHeapCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetDescriptorHeapCacheData(renderData.constantBuffer);

		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = globalRenderData.depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = globalRenderData.swapChain->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = vertexUploadCacheData.mUploadBuffer->BufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = indexUploadCacheData.mUploadBuffer->IndexBufferView();

		// Specify the buffers we are going to render to.
		mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);
		ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeapCacheData.descriptorHeap->GetDescriptorHeap().Get() };
		mCommandList->SetDescriptorHeaps((UINT)_countof(descriptorHeaps), descriptorHeaps);
		mCommandList->SetGraphicsRootSignature(shaderCacheData.shaderData.RootSignature->GetRootSignature().Get());
		mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		mCommandList->IASetIndexBuffer(&indexBufferView);
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetPipelineState(shaderCacheData.shaderPSO->GetDX12PSO().Get());
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeapCacheData.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());

		const size_t count = constantBufferResourceHandle->mCount;
		cbvHandle.Offset((UINT)((globalRenderData.device->GetCbvSrvDescriptorSize() * count) * globalRenderData.globalRenderFrameContext->GetCurrentFrameResource()));

		for (int i = 0; i < count; ++i)
		{
			if (const blCompositeMeshResource::MeshEntry* entry =
				renderData.compositeMesh->FindMeshEntry(renderData.submeshId))
			{
				mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

				mCommandList->DrawIndexedInstanced(
					(UINT)entry->mIndexCount,
					1,
					(UINT)entry->mIndexCountOffset,
					(UINT)entry->mVertexCountOffset,
					0);
			}

			cbvHandle.Offset(globalRenderData.device->GetCbvSrvDescriptorSize());
		}
	}
}