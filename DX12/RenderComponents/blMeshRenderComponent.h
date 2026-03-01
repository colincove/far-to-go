#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blMeshRenderComponent : public blRenderComponent<RenderMeshData>
	{
	public:
		blMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr);
		virtual ~blMeshRenderComponent() = default;
		virtual void Render(const RenderMeshData& item, const blSceneResourcePtr scene) override;
	};
}