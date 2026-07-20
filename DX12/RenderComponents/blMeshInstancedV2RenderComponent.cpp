#include <blMeshInstancedV2RenderComponent.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>
#include <blGlobalRenderData.h>
#include <blDX12MeshGeometry.h>

namespace
{
	using namespace BoulderLeaf;
	using namespace BoulderLeaf::Graphics;
	using namespace BoulderLeaf::Graphics::DX12;

	// V2: instance data is owned by the blConstantBufferResource (constantBuffers).
	// We'll extract the instance array buffer and its translated resources when needed.
}

namespace BoulderLeaf::Graphics::DX12
{
	blMeshInstancedV2RenderComponent::blMeshInstancedV2RenderComponent(
		blGlobalRenderData* globalRenderDataPtr)
		: blRenderComponent(globalRenderDataPtr, L"blMeshInstancedV2RenderComponent"),
		mBufferUploadCache(globalRenderDataPtr->device.get(), mCommandList.get())
	{

	}

	void blMeshInstancedV2RenderComponent::Render(const RenderMeshDataInstancedV2& renderData)
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
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = globalRenderData.depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = globalRenderData.swapChain->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = vertexUploadCacheData.mUploadBuffer->BufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = indexUploadCacheData.mUploadBuffer->IndexBufferView();

		// Specify the buffers we are going to render to.
		mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);

		mCommandList->SetGraphicsRootSignature(shaderCacheData.shaderData.RootSignature->GetRootSignature().Get());
		mCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		mCommandList->IASetIndexBuffer(&indexBufferView);
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetPipelineState(shaderCacheData.shaderPSO->GetDX12PSO().Get());

		const UINT cbvDescSize = globalRenderData.device->GetCbvSrvDescriptorSize();
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE();
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvPassConstantsStartHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE();
		size_t count = 0;

		if (renderData.constantBuffers.IsValid())
		{
			const blDX12ConstantBufferDescriptorHeapCacheData& constantBufferDescriptorHeap =
				globalRenderData.resourceCacheGlobalInterface->GetConstantBufferDescriptorHeapCacheData(renderData.constantBuffers);

			ID3D12DescriptorHeap* descriptorHeaps[] = { constantBufferDescriptorHeap.descriptorHeap->GetDescriptorHeap().Get() };
			mCommandList->SetDescriptorHeaps((UINT)_countof(descriptorHeaps), descriptorHeaps);

			cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferDescriptorHeap.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
			cbvPassConstantsStartHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(constantBufferDescriptorHeap.descriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());

			const UINT cbvStartOfFrameResouceOffset = (cbvDescSize * (constantBufferDescriptorHeap.instanceCount + constantBufferDescriptorHeap.numberOfConstantBuffers))
				* globalRenderData.globalRenderFrameContext->GetCurrentFrameResource();
			cbvHandle.Offset(cbvStartOfFrameResouceOffset);
			cbvHandle.Offset(cbvDescSize * constantBufferDescriptorHeap.numberOfConstantBuffers);
			cbvPassConstantsStartHandle.Offset(cbvStartOfFrameResouceOffset);

			// Determine instance count from the instance constant buffer contained inside constantBuffers
			auto cbContainer = renderData.constantBuffers.GetContainer();
			const blResourceHandleOfType<blArrayBufferResource> instanceArrayBufferResource =
				cbContainer->CreateHandleFromRefOfType<blArrayBufferResource>(renderData.constantBuffers->mInstanceConstantBuffer);

			const blDX12ArrayBufferTranslationCacheData& instanceTranslationCacheData =
				globalRenderData.resourceCacheGlobalInterface->GetArrayBufferTranslationCacheData(instanceArrayBufferResource);

			const blResourceHandleOfType<blListResource>& instanceListResource = cbContainer->CreateHandleFromRefOfType<blListResource>(
				instanceTranslationCacheData.translatedArrayBuffer->mBufferResourceRef);

			count = instanceListResource->mCount;
		}

		if (count == 0)
		{
			// Nothing to draw
			return;
		}

		for (int i = 0; i < (int)count; ++i)
		{
			mCommandList->SetGraphicsRootDescriptorTable(0, cbvHandle);

			if (renderData.constantBuffers.IsValid())
			{
				auto cbvPassConstantsHandle =
					CD3DX12_GPU_DESCRIPTOR_HANDLE(cbvPassConstantsStartHandle);

				for (int i = 0; i < renderData.constantBuffers->mPassConstantBuffers.mCount; ++i)
				{
					mCommandList->SetGraphicsRootDescriptorTable(i + 1, cbvPassConstantsHandle);
					cbvPassConstantsHandle.Offset(cbvDescSize);
				}
			}

			mCommandList->DrawIndexedInstanced(
				(UINT)indexListResource->mCount,
				1, 0, 0, 0);
			cbvHandle.Offset(globalRenderData.device->GetCbvSrvDescriptorSize());
		}
	}
}
