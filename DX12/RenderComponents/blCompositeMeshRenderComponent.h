#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderComponent : public blRenderComponent<RenderCompositeMeshDataInstanced>
	{
	private:
		std::shared_ptr<blCompositeMeshDataCache> mCompositeMeshStorageCache;// I THINK this can be global.
		std::shared_ptr<blDX12MeshDataDeviceCache> mMeshDataDeviceCache; //this uploads to a command list. not global. 
	public:
		blCompositeMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr);
		virtual ~blCompositeMeshRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataInstanced& item, const blSceneResourcePtr scene) override;
	};
}