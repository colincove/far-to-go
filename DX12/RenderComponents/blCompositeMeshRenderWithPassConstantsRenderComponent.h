#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderWithPassConstantsRenderComponent : public blRenderComponent<RenderCompositeMeshDataWithPassConstants>
	{
	private:
		std::shared_ptr<blCompositeMeshDataCache> mCompositeMeshStorageCache;// I THINK this can be global.
		std::shared_ptr<blDX12MeshDataDeviceCache> mMeshDataDeviceCache; //this uploads to a command list. not global. 
	public:
		blCompositeMeshRenderWithPassConstantsRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr);
		virtual ~blCompositeMeshRenderWithPassConstantsRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataWithPassConstants& item, const blSceneResourcePtr scene) override;
	};
}