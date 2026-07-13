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
		std::unique_ptr<blConstantBufferDescriptorHeap> descriptorHeap;
	};
	
	class blDX12ConstantBufferCache : 
		public blDX12ResourceDataCache<blDX12ConstantBufferCacheData, blDataBufferInterfaceResource>
	{
	private:
		blDevice* mDevice;
		blDX12BufferDataCache* mBufferCache;
		blGlobalRenderFrameContext* mGlobalRenderFrameContext;
	public:
		blDX12ConstantBufferCache(
			blDevice* device,
			blDX12BufferDataCache* mBufferCache,
			blGlobalRenderFrameContext* globalRenderFrameContext);

	protected:
		virtual void InitializeCache(
			const blDataBufferInterfaceResource& resourcePtr,
			blDX12ConstantBufferCacheData& cache) override;
	};
}