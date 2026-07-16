#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderComponent : public blRenderComponent<RenderCompositeMeshDataInstanced>
	{
	private:
		blDX12UploadBufferCache mBufferUploadCache;
	public:
		blCompositeMeshRenderComponent(blGlobalRenderData* globalRenderDataPtr);
		virtual ~blCompositeMeshRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataInstanced& item) override;
	};
}