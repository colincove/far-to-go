#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blMeshInstancedRenderComponent : public blRenderComponent<RenderMeshDataInstanced>
	{
	public:
		blMeshInstancedRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr);
		virtual ~blMeshInstancedRenderComponent() = default;
		virtual void Render(const RenderMeshDataInstanced& item, const blSceneResourcePtr scene) override;
	};
}