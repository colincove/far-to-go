#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blMeshInstancedRenderComponent : public blRenderComponent<RenderMeshDataInstanced>
	{
	private:
		std::shared_ptr<blDX12MeshDataDeviceCache> mMeshDataDeviceCache;
	public:
		blMeshInstancedRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr);
		virtual ~blMeshInstancedRenderComponent() = default;
		virtual void Render(const RenderMeshDataInstanced& item, const blSceneResourcePtr scene) override;
	};
}