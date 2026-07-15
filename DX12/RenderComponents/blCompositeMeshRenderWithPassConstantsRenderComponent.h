#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderWithPassConstantsRenderComponent : public blRenderComponent<RenderCompositeMeshDataWithPassConstants>
	{
	private:
		std::unique_ptr<blCompositeMeshDataCache> mCompositeMeshStorageCache;// I THINK this can be global.
		std::unique_ptr<blDX12MeshDataDeviceCache> mMeshDataDeviceCache; //this uploads to a command list. not global. 
	public:
		blCompositeMeshRenderWithPassConstantsRenderComponent(blGlobalRenderData* globalRenderDataPtr);
		virtual ~blCompositeMeshRenderWithPassConstantsRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataWithPassConstants& item) override;
	};
}