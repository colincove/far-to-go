#pragma once

#include <memory>
#include <blDX12ResourceDataCache.h>
#include <blBuffer.h>
#include <blGlobalRenderFrameContext.h>
#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12VertexAndPassUploadBufferCacheData
	{
		
	};

	class blDX12VertexAndPassUploadBufferCache : public blDX12ResourceDataCache<blDX12VertexAndPassUploadBufferCacheData, blVertexBufferWithPassBufferResource>
	{
	public:
		blDX12VertexAndPassUploadBufferCache(blDevice* device,
			blGlobalRenderFrameContext* globalRenderFrameContext);
		virtual void UpdateCache(const blResourceId& resourceId) override;
	protected:
		virtual void InitializeCache(
			const blVertexBufferWithPassBufferResource& resource,
			blDX12VertexAndPassUploadBufferCacheData& cache) override;
	};
}