#pragma once

#include <RenderComponents/blRenderComponent.h>
#include <RenderItems/blRenderItemMesh.h>
#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blMeshInstancedRenderComponent : public blRenderComponent<RenderMeshDataInstanced>
	{
	private:
		blDX12UploadBufferCache mBufferUploadCache;
	public:
		blMeshInstancedRenderComponent(blGlobalRenderData* globalRenderDataPtr);
		virtual ~blMeshInstancedRenderComponent() = default;
		virtual void Render(const RenderMeshDataInstanced& item) override;
	};
}