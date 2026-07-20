#include <blCompositeMeshRenderWithPassConstantsRenderComponent.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>
#include <blDX12Buffer.h>
#include <blDX12MeshGeometry.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshRenderWithPassConstantsRenderComponent::blCompositeMeshRenderWithPassConstantsRenderComponent(blGlobalRenderData* globalRenderDataPtr) :
		blRenderComponent(globalRenderDataPtr),
		mBufferUploadCache(globalRenderDataPtr->device.get(), mCommandList.get())
	{
	}

	void blCompositeMeshRenderWithPassConstantsRenderComponent::Render(const RenderCompositeMeshDataWithPassConstants& renderData)
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
			renderData.instanceConstantBuffer->mBufferResourceRef);
		const blDX12MappedUploadBufferCacheData& constantBufferUploadBufferCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetMappedUploadBufferCache(renderData.instanceConstantBuffer);

		const blResourceHandleOfType<blListResource> constantBufferResourceHandle =
			renderData.instanceConstantBuffer.GetContainer()->CreateHandleFromRefOfType<blListResource>(renderData.instanceConstantBuffer->mBufferResourceRef);
		const blDX12DescriptorHeapCacheData& descriptorHeapCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetDescriptorHeapCacheData(renderData.instanceConstantBuffer);

		const blResourceHandleOfType<blListResource> passConstantBufferListResource = container->CreateHandleFromRefOfType<blListResource>(
			renderData.passConstantBuffer->mBufferResourceRef);
		const blDX12MappedUploadBufferCacheData& passBufferUploadBufferCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetMappedUploadBufferCache(renderData.passConstantBuffer);

		const blResourceHandleOfType<blListResource> passConstantBufferResourceHandle =
			renderData.passConstantBuffer.GetContainer()->CreateHandleFromRefOfType<blListResource>(renderData.passConstantBuffer->mBufferResourceRef);
		const blDX12DescriptorHeapCacheData& passDescriptorHeapCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetDescriptorHeapCacheData(renderData.passConstantBuffer);

		const blDX12ConstantBufferDescriptorHeapCacheData& constantBufferDescriptorHeap =
			globalRenderData.resourceCacheGlobalInterface->GetConstantBufferDescriptorHeapCacheData(renderData.mConstantBuffers);

		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = globalRenderData.depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = globalRenderData.swapChain->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = vertexUploadCacheData.mUploadBuffer->BufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = indexUploadCacheData.mUploadBuffer->IndexBufferView();

		// Specify the buffers we are going to render to.
		mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);
		ID3D12DescriptorHeap* descriptorHeaps[] = { 
			constantBufferDescriptorHeap.descriptorHeap->GetDescriptorHeap().Get()
		};
		mCommandList->SetDescriptorHeaps((UINT)_countof(descriptorHeaps), descriptorHeaps);
		mCommandList->SetGraphicsRootSignature(shaderCacheData.shaderData.RootSignature->GetRootSignature().Get());
		mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		mCommandList->IASetIndexBuffer(&indexBufferView);
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetPipelineState(shaderCacheData.shaderPSO->GetDX12PSO().Get());

		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferDescriptorHeap.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
		auto cbvPassConstantsHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferDescriptorHeap.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());

		const UINT cbvDescSize = globalRenderData.device->GetCbvSrvDescriptorSize();
		const UINT cbvStartOfFrameResouceOffset = (cbvDescSize * (constantBufferDescriptorHeap.instanceCount + constantBufferDescriptorHeap.numberOfConstantBuffers))
			* globalRenderData.globalRenderFrameContext->GetCurrentFrameResource();
		cbvHandle.Offset(cbvStartOfFrameResouceOffset);
		cbvHandle.Offset(cbvDescSize * constantBufferDescriptorHeap.numberOfConstantBuffers);
		cbvPassConstantsHandle.Offset(cbvStartOfFrameResouceOffset);

		for (const BoulderLeaf::blResourceId& resourceId : renderData.submeshIds)
		{
			if (const blCompositeMeshResource::MeshEntry* entry =
				renderData.compositeMesh->FindMeshEntry(resourceId))
			{
				mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
				mCommandList->SetGraphicsRootDescriptorTable(1, cbvPassConstantsHandle);

				mCommandList->DrawIndexedInstanced(
					(UINT)entry->mIndexCount,
					1,
					(UINT)entry->mIndexCountOffset,
					(UINT)entry->mVertexCountOffset,
					0);
			}

			cbvHandle.Offset(cbvDescSize);
		}
	}
}