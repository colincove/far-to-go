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
		std::shared_ptr<blConstantBuffer> ConstantBuffer;
	};
	
	class blDX12ConstantBufferCache : 
		//public blDX12ResourceDataCache<blDX12ConstantBufferCacheData, blDataBufferInterfaceResource>
		public blDX12ResourceDataCache<blDX12ConstantBufferCacheData, blStandardObjectConstantsBufferResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blDX12BufferDataCache> mBufferCache;
		std::shared_ptr<blConstantBufferDescriptorHeap> mCbvHeap;
		std::shared_ptr<blGlobalRenderFrameContext> mGlobalRenderFrameContext;
	public:
		blDX12ConstantBufferCache(
			std::shared_ptr<blDevice> device,
			std::shared_ptr<blConstantBufferDescriptorHeap> cbvHeap,
			std::shared_ptr<blDX12BufferDataCache> mBufferCache,
			std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext);

	protected:
		virtual void InitializeCache(
			const blStandardObjectConstantsBufferResource& resourcePtr,
			blDX12ConstantBufferCacheData& cache) override;
	};
}