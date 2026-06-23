#pragma once

#include <vector>
#include <memory>
#include <blDevice.h>
#include <blDX12ResourceDataCache.h>
#include <blBuffer.h>
#include <blConstantBuffer.h>
#include <blDX12BufferData.h>
#include <blConstantBufferDescriptorHeap.h>
#include <blBufferTypes.h>
#include <blGlobalRenderFrameContext.h>
#include <blDX12ConstantBufferCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12ConstantBufferWithPassCache :
		public blDX12ResourceDataCache<blDX12ConstantBufferCacheData, blVertexBufferWithPassBufferResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blDX12BufferDataCache> mBufferCache;
		std::shared_ptr<blGlobalRenderFrameContext> mGlobalRenderFrameContext;
	public:
		blDX12ConstantBufferWithPassCache(
			std::shared_ptr<blDevice> device,
			std::shared_ptr<blDX12BufferDataCache> mBufferCache,
			std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext);

	protected:
		virtual void InitializeCache(
			const blVertexBufferWithPassBufferResource& resourcePtr,
			blDX12ConstantBufferCacheData& cache) override;
	};
}