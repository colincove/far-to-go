#include <blCompositeMeshRenderWithPassConstantsRenderComponent.h>

#include <RenderData/blDX12CompositeMeshDataCache.h>
#include <blDX12Buffer.h>

#include <blDX12ConstantBufferWithPassCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshRenderWithPassConstantsRenderComponent::blCompositeMeshRenderWithPassConstantsRenderComponent(blGlobalRenderData* globalRenderDataPtr) :
		blRenderComponent(globalRenderDataPtr),
		mCompositeMeshStorageCache(std::make_unique<blCompositeMeshDataCache>(
			globalRenderDataPtr->device.get(),
			mCommandList.get(),
			globalRenderDataPtr->meshStorageCache.get())),
		mMeshDataDeviceCache(std::make_unique<blDX12MeshDataDeviceCache>(
			globalRenderDataPtr->device.get(),
			mCommandList.get(),
			globalRenderDataPtr->meshStorageCache.get()))
	{
	}

	void blCompositeMeshRenderWithPassConstantsRenderComponent::Render(const RenderCompositeMeshDataWithPassConstants& renderData, const blSceneResourcePtr scene)
	{
		if (!renderData.compositeMesh)
		{
			return;
		}

		blGlobalRenderData& globalRenderData = *mGlobalRenderData;

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
		//const blDX12ConstantBufferCacheData& constantBufferCache = globalRenderData.constantBufferCache->Get(*renderData.constantBuffer.get());
		const blDX12ConstantBufferCacheData& constantBufferCacheWithPass = globalRenderData.constantBufferWithPassCache->Get(*renderData.constantBufferWithPass.get());

		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = globalRenderData.depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = globalRenderData.swapChain->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = meshDeviceStorageCacheData.meshGeometry.VertexBufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = meshDeviceStorageCacheData.meshGeometry.IndexBufferView();

		// Specify the buffers we are going to render to.
		mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);
		ID3D12DescriptorHeap* descriptorHeaps[] = { constantBufferCacheWithPass.descriptorHeap->GetDescriptorHeap().Get() };
		mCommandList->SetDescriptorHeaps((UINT)_countof(descriptorHeaps), descriptorHeaps);
		mCommandList->SetGraphicsRootSignature(shaderCacheData.RootSignature->GetRootSignature().Get());
		mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		mCommandList->IASetIndexBuffer(&indexBufferView);
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetPipelineState(psoCacheData.PSO->GetDX12PSO().Get());
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferCacheWithPass.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
		auto cbvPassConstantsHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferCacheWithPass.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
		

		const size_t count = renderData.constantBuffer->GetData().Count();
		cbvHandle.Offset((UINT)((globalRenderData.device->GetCbvSrvDescriptorSize() * (count + 1)) * globalRenderData.globalRenderFrameContext->GetCurrentFrameResource()));
		cbvHandle.Offset(globalRenderData.device->GetCbvSrvDescriptorSize());//the descriptor table starts with the pass constants. Need to offset for the objects
		cbvPassConstantsHandle.Offset((UINT)((globalRenderData.device->GetCbvSrvDescriptorSize() * (count + 1)) * globalRenderData.globalRenderFrameContext->GetCurrentFrameResource()));

		// first. Just render the single mesh that was handed in
		blMeshIndexedCatalogue::Entry entry = renderData.compositeMesh->GetData().GetMeshEntry(renderData.submeshId);
		mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
		mCommandList->SetGraphicsRootDescriptorTable(1, cbvPassConstantsHandle);

		mCommandList->DrawIndexedInstanced(
			(UINT)entry.IndexCount,
			1,
			(UINT)entry.IndexOffset,
			(UINT)entry.VertexOffset,
			0);

		cbvHandle.Offset(globalRenderData.device->GetCbvSrvDescriptorSize());

		//and then render the list of sub mesh's passed in
		for (const blResourceId& resourceId : renderData.submeshIds)
		{
			blMeshIndexedCatalogue::Entry entry = renderData.compositeMesh->GetData().GetMeshEntry(resourceId);
			mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);
			mCommandList->SetGraphicsRootDescriptorTable(1, cbvPassConstantsHandle);

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