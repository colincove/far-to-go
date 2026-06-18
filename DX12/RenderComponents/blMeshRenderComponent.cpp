#include <blMeshRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	blMeshRenderComponent::blMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr)
		: blRenderComponent(globalRenderDataPtr)
	{
	}

	void blMeshRenderComponent::Render(const RenderMeshData& renderData, const  blSceneResourcePtr scene)
	{
		if (renderData.material.expired())
		{
			return;
		}

		blMaterialResourcePtr materialResourcePtr = renderData.material.lock();
		const blMaterial& material = materialResourcePtr->GetData();
		const blShaderResource& shaderResource = *material.shader;
		const blShader& shader = shaderResource.GetData();
		const blShaderCacheData& shaderCacheData = mGlobalRenderDataPtr->shaderCache->Get(shaderResource);

		if (renderData.mesh.expired())
		{
			return;
		}

		blMeshBaseResourcePtr meshBaseResourcePtr = renderData.mesh.lock();
		const blMeshBaseResource& meshResource = *meshBaseResourcePtr.get();
		const blMeshBase& mesh = meshBaseResourcePtr->GetData();
		const blDX12MeshStorageCacheData& meshCacheData = mGlobalRenderDataPtr->meshStorageCache->Get(meshResource);

		blRenderGroupData& renderGroupData = mGlobalRenderDataPtr->renderGroupData[renderData.group];
		const blDX12MeshDataDeviceCacheData& deviceCache = renderGroupData.meshDataDeviceCache->Get(meshResource);
		const blPSOCacheData& psoCacheData = mGlobalRenderDataPtr->mPSOCache->Get(shaderResource);
		const blPSO& pso = *psoCacheData.PSO;

		ComPtr<ID3D12GraphicsCommandList> commandList = renderGroupData.commandList->GetCommandListPtr();

		commandList->SetPipelineState(pso.GetDX12PSO().Get());
	}
}