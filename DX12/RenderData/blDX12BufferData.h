#pragma once

#include <vector>
#include <memory>
#include <blBuffer.h>
#include <blDevice.h>
#include <blDX12ResourceDataCache.h>
#include <blDX12Math.inl>
#include "../DirectX-Headers/include/directx/d3dx12.h"
#include <blBufferTypes.h>
#include <blGlobalRenderFrameContext.h>
#include <blConstantBufferDescriptorHeap.h>
#include <blUploadBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12BufferData
	{
		std::shared_ptr<blBasicDataBuffer> dataBuffer;
		std::unique_ptr<blDX12blVertexDataUploadBuffer> uploadBuffer;
	};

	class blDX12BufferDataCache : 
		public blDX12ResourceDataCache<blDX12BufferData, blDataBufferInterfaceResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blGlobalRenderFrameContext> mGlobalRenderFrameContext;
	public:
		blDX12BufferDataCache(std::shared_ptr<blDevice> device, 
			std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext);
		virtual void UpdateCache(const blResourceId& resourceId) override;
	protected:
		virtual void InitializeCache(
			const blDataBufferInterfaceResource& resource,
			blDX12BufferData& cache) override;
	};
}