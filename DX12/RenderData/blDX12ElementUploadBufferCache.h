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
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blGlobalRenderFrameContext> mGlobalRenderFrameContext;
		std::shared_ptr<blDX12BufferElementCache> mElementCache;
	public:
		blDX12ElementUploadBufferCache(std::shared_ptr<blDevice> device,
			std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext,
			std::shared_ptr<blDX12BufferElementCache> elementCache);
		virtual void UpdateCache(const blResourceId& resourceId) override;
	protected:
		virtual void InitializeCache(
			const blDataElementBufferResource& resource,
			blDX12ElementUploadBufferCacheData& cache) override;
	};
}
