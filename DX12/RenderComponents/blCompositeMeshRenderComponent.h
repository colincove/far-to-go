#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderComponent : public blRenderComponent<RenderCompositeMeshDataInstanced>
	{
	public:
		blCompositeMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr);
		virtual ~blCompositeMeshRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataInstanced& item, const blSceneResourcePtr scene) override;
	};
}