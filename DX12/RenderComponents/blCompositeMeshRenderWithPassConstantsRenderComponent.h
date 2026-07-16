#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <blGlobalRenderData.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCompositeMeshRenderWithPassConstantsRenderComponent : public blRenderComponent<RenderCompositeMeshDataWithPassConstants>
	{
	private:
		blDX12UploadBufferCache mBufferUploadCache;
	public:
		blCompositeMeshRenderWithPassConstantsRenderComponent(blGlobalRenderData* globalRenderDataPtr);
		virtual ~blCompositeMeshRenderWithPassConstantsRenderComponent() = default;
		virtual void Render(const RenderCompositeMeshDataWithPassConstants& item) override;
	};
}