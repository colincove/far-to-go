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
#include <RenderData/blDX12BufferElementDataCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12ElementUploadBufferCache;

	class blDX12ConstantBufferWithPassCache :
		public blDX12ResourceDataCache<blDX12ConstantBufferCacheData, blVertexBufferWithPassBufferResource>
	{
	private:
		blDevice* mDevice;
		blDX12BufferDataCache* mBufferCache;
		blDX12BufferElementCache* mBufferElementCache;
		blGlobalRenderFrameContext* mGlobalRenderFrameContext;
		blDX12ElementUploadBufferCache* mElementUploadBufferCache;
	public:
		blDX12ConstantBufferWithPassCache(
			blDevice* device,
			blDX12BufferDataCache* mBufferCache,
			blDX12BufferElementCache* bufferElementCache,
			blGlobalRenderFrameContext* globalRenderFrameContext,
			blDX12ElementUploadBufferCache* elementUploadBufferCache);

	protected:
		virtual void InitializeCache(
			const blVertexBufferWithPassBufferResource& resourcePtr,
			blDX12ConstantBufferCacheData& cache) override;
	};
}