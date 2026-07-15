#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderComponent : public blRenderComponent<RenderCompositeMeshDataInstanced>
	{
	private:
		std::unique_ptr<blCompositeMeshDataCache> mCompositeMeshStorageCache;// I THINK this can be global.
		std::unique_ptr<blDX12MeshDataDeviceCache> mMeshDataDeviceCache; //this uploads to a command list. not global. 
	public:
		blCompositeMeshRenderComponent(blGlobalRenderData* globalRenderDataPtr);
		virtual ~blCompositeMeshRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataInstanced& item) override;
	};
}