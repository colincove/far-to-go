#include <blMeshInstancedRenderComponent.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>
#include <blGlobalRenderData.h>
#include <blDX12MeshGeometry.h>

namespace BoulderLeaf::Graphics::DX12
{
	blMeshInstancedRenderComponent::blMeshInstancedRenderComponent(
		blGlobalRenderData* globalRenderDataPtr)
		: blRenderComponent(globalRenderDataPtr, L"blMeshInstancedRenderComponent"),
		mBufferUploadCache(globalRenderDataPtr->device.get(), mCommandList.get())
	{

	}

	void blMeshInstancedRenderComponent::Render(const RenderMeshDataInstanced& renderData)
	{
		blGlobalRenderData& globalRenderData = *mGlobalRenderData;
		blResourceContainer* container = renderData.mesh.GetContainer();

		const blResourceHandleOfType<blIndexedMeshResource> meshResource = renderData.mesh;
		const blResourceHandleOfType<blArrayBufferResource> meshArrayBufferResource = meshResource.GetContainer()->CreateHandleFromRefOfType<blArrayBufferResource>(meshResource->mArrayBufferResourceRef);
		const blDX12ArrayBufferTranslationCacheData& meshTranslationCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetArrayBufferTranslationCacheData(meshArrayBufferResource);
		const blResourceHandleOfType<blListResource>& translatedVertexListResource = container->CreateHandleFromRefOfType<blListResource>(
			meshTranslationCacheData.translatedArrayBuffer->mBufferResourceRef);
		const blResourceHandleOfType<blListResource>& indexListResource = container->CreateHandleFromRefOfType<blListResource>(
			meshResource->mIndexListRef);

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
		uint32_t version = container->GetResourceVersion(constantBufferListResource.GetId());
		const blDX12MappedUploadBufferCacheData& constantBufferUploadBufferCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetMappedUploadBufferCache(constantBufferListResource);

		const blResourceHandleOfType<blListResource> constantBufferResourceHandle =
			renderData.constantBuffer.GetContainer()->CreateHandleFromRefOfType<blListResource>(renderData.constantBuffer->mBufferResourceRef);
		const blDX12DescriptorHeapCacheData& descriptorHeapCacheData = globalRenderData.resourceCacheGlobalInterface->
			GetDescriptorHeapCacheData(constantBufferResourceHandle);

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
			mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
			mCommandList->DrawIndexedInstanced(
				(UINT)indexListResource->mCount,
				1, 0, 0, 0);
			cbvHandle.Offset(globalRenderData.device->GetCbvSrvDescriptorSize());
		}
	}
}