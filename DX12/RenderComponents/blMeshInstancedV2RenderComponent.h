#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blMeshInstancedV2RenderComponent : public blRenderComponent<RenderMeshDataInstancedV2>
	{
	private:
		blDX12UploadBufferCache mBufferUploadCache;
	public:
		blMeshInstancedV2RenderComponent(blGlobalRenderData* globalRenderDataPtr);
		virtual ~blMeshInstancedV2RenderComponent() = default;
		virtual void Render(const RenderMeshDataInstancedV2& item) override;
	};
}
