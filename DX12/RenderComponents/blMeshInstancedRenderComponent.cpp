#include <blMeshInstancedRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	blMeshInstancedRenderComponent::blMeshInstancedRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr)
		: blRenderComponent(globalRenderDataPtr)
	{
	}

	void blMeshInstancedRenderComponent::Render(const RenderMeshDataInstanced& renderData, const  blSceneResourcePtr scene)
	{
		blGlobalRenderData& globalRenderData = *mGlobalRenderDataPtr.get();
		blRenderGroupData& groupData = globalRenderData.renderGroupData[renderData.group];

		const blMaterial& material = renderData.material->GetData();
		const blShaderResource& shaderResource = *material.shader;
		const blShader& shader = shaderResource.GetData();
		const blShaderCacheData& shaderCacheData = globalRenderData.shaderCache->Get(shaderResource);

		const blMeshBaseResource& meshResource = *renderData.mesh;
		const blMeshBase& mesh = meshResource.GetData();
		const blDX12MeshStorageCacheData& meshCacheData = globalRenderData.meshStorageCache->Get(meshResource);
		const blDX12MeshDataDeviceCacheData& meshDeviceCacheData = groupData.meshDataDeviceCache->Get(meshResource);
		const blPSOCacheData& psoCacheData = globalRenderData.mPSOCache->Get(shaderResource);

		globalRenderData.mCurrentPSO = psoCacheData.PSO;

		// Do I not do anything with this data? How do constant buffers get bound? I'm confused. 
		//TODO: Why is this data specific to standard object constants? I want to be able to use this for any type of constant buffer.
		const blDX12ConstantBufferCacheData& constantBufferCache = globalRenderData.constantBufferCache->Get(*renderData.constantBuffer.get());
		
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = globalRenderData.depthBuffer->DepthStencilView();
		D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = globalRenderData.swapChain->CurrentBackBufferView();
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = meshDeviceCacheData.meshGeometry.VertexBufferView();
		D3D12_INDEX_BUFFER_VIEW indexBufferView = meshDeviceCacheData.meshGeometry.IndexBufferView();

		// Specify the buffers we are going to render to.
		ID3D12GraphicsCommandList& commandList = *groupData.commandList->GetCommandListPtr().Get();
		commandList.OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);
		ID3D12DescriptorHeap* descriptorHeaps[] = { globalRenderData.constantBufferDescriptorHeap->GetDescriptorHeap().Get()};
		commandList.SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		commandList.SetGraphicsRootSignature(shaderCacheData.RootSignature->GetRootSignature().Get());
		commandList.IASetVertexBuffers(0, 1, &vertexBufferView);
		commandList.IASetIndexBuffer(&indexBufferView);
		commandList.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList.SetGraphicsRootDescriptorTable(0, globalRenderData.constantBufferDescriptorHeap->GetDescriptorHeap().Get()->GetGPUDescriptorHandleForHeapStart());
		commandList.SetPipelineState(psoCacheData.PSO->GetDX12PSO().Get());
		commandList.DrawIndexedInstanced(
			meshCacheData.meshStorage.GetIndexCount(),
			renderData.constantBuffer->GetData().Count(), 0, 0, 0);
	}
}