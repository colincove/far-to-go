#include <blMeshInstancedRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	blMeshInstancedRenderComponent::blMeshInstancedRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr)
		: blRenderComponent(globalRenderDataPtr)
	{
	}

	void blMeshInstancedRenderComponent::Render(const RenderMeshDataInstanced& renderData, const  blSceneResourcePtr scene)
	{
		const blMaterial& material = renderData.material->GetData();
		const blShaderResource& shaderResource = *material.shader;
		const blShader& shader = shaderResource.GetData();
		const blShaderCacheData& shaderCacheData = mGlobalRenderDataPtr->shaderCache->Get(shaderResource);

		const blMeshBaseResource& meshResource = *renderData.mesh;
		const blMeshBase& mesh = meshResource.GetData();
		const blDX12MeshStorageCacheData& meshCacheData = mGlobalRenderDataPtr->meshStorageCache->Get(meshResource);
	}
}