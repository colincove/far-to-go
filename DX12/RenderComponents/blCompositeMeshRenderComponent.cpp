#include <blCompositeMeshRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCompositeMeshRenderComponent::blCompositeMeshRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr) :
		blRenderComponent(globalRenderDataPtr)
	{
	}

	void blCompositeMeshRenderComponent::Render(const RenderCompositeMeshDataInstanced& item, const blSceneResourcePtr scene)
	{
		//TODO render
	}
}