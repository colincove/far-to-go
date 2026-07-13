#pragma once

#include <memory>
#include <blDX12ResourceDataCache.h>
#include <blBuffer.h>
#include <blDevice.h>
#include <blDX12BufferElementDataCache.h>
#include <blGlobalRenderFrameContext.h>
#include <blUploadBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ElementUploadBufferCacheData
	{
		std::unique_ptr<blDX12ElementUploadBuffer> uploadBuffer;
	};

	class blDX12ElementUploadBufferCache : public blDX12ResourceDataCache<blDX12ElementUploadBufferCacheData, blDataElementBufferResource>
	{
	private:
		blDevice* mDevice;
		blGlobalRenderFrameContext* mGlobalRenderFrameContext;
		blDX12BufferElementCache* mElementCache;
	public:
		blDX12ElementUploadBufferCache(blDevice* device,
			blGlobalRenderFrameContext* globalRenderFrameContext,
			blDX12BufferElementCache* elementCache);
		virtual void UpdateCache(const blResourceId& resourceId) override;
	protected:
		virtual void InitializeCache(
			const blDataElementBufferResource& resource,
			blDX12ElementUploadBufferCacheData& cache) override;
	};
}
