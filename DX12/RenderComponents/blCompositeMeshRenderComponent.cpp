#include <blCompositeMeshRenderComponent.h>

#include <RenderData/blDX12CompositeMeshDataCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshRenderComponent::blCompositeMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr) :
		blRenderComponent(globalRenderDataPtr)
	{
	}

	void blCompositeMeshRenderComponent::Render(const RenderCompositeMeshDataInstanced& item, const blSceneResourcePtr scene)
	{
		if (!item.compositeMesh)
		{
			return;
		}

		blRenderGroupData& groupData = mGlobalRenderData->renderGroupData[item.group];

		//TODO render
		const blCompositeMeshDataCacheData& cache = groupData.compositeMeshStorageCache->Get(
			*item.compositeMesh
		);
	}
}