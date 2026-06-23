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

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ConstantBufferCacheData
	{
		std::shared_ptr<blConstantBufferDescriptorHeap> descriptorHeap;
	};
	
	class blDX12ConstantBufferCache : 
		public blDX12ResourceDataCache<blDX12ConstantBufferCacheData, blDataBufferInterfaceResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blDX12BufferDataCache> mBufferCache;
		std::shared_ptr<blGlobalRenderFrameContext> mGlobalRenderFrameContext;
	public:
		blDX12ConstantBufferCache(
			std::shared_ptr<blDevice> device,
			std::shared_ptr<blDX12BufferDataCache> mBufferCache,
			std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext);

	protected:
		virtual void InitializeCache(
			const blDataBufferInterfaceResource& resourcePtr,
			blDX12ConstantBufferCacheData& cache) override;
	};
}