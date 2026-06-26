#include <blDX12ElementUploadBufferCache.h>
#include <Resources/blResourceManager.h>
#include <blDX12BufferElementDataCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ElementUploadBufferCache::blDX12ElementUploadBufferCache(std::shared_ptr<blDevice> device,
		std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext,
		std::shared_ptr<blDX12BufferElementCache> elementCache)
		: mDevice(device), mGlobalRenderFrameContext(globalRenderFrameContext), mElementCache(elementCache), blDX12ResourceDataCache<blDX12ElementUploadBufferCacheData, blDataElementBufferResource>()
	{
	}

	void blDX12ElementUploadBufferCache::InitializeCache(
		const blDataElementBufferResource& resource,
		blDX12ElementUploadBufferCacheData& cache)
	{
		// Get the DX12-adapted element buffer from the element cache
		auto& elementCache = mElementCache->Get(resource);
		if (!elementCache.data)
		{
			assert(false);
			return;
		}

		cache.uploadBuffer = std::make_unique<blDX12ElementUploadBuffer>(
			mDevice,
			true,
			elementCache.data.get(),
			resource.GetName()
		);

		cache.uploadBuffer->Initialize();
		cache.uploadBuffer->CopyAllData();
	}

	void blDX12ElementUploadBufferCache::UpdateCache(const blResourceId& resourceId)
	{
		blDX12ElementUploadBufferCacheData& cache = GetByIdNoInit(resourceId);

		const blDataElementBufferResourcePtr resourcePtr = blResourceManager::Get()
			.GetResource<blDataElementBufferResource>(resourceId);

		if (!cache.uploadBuffer)
		{
			auto& elementCache = mElementCache->GetByIdNoInit(resourceId);
			if (!elementCache.data)
			{
				assert(false);
				return;
			}

			cache.uploadBuffer = std::make_unique<blDX12ElementUploadBuffer>(
				mDevice,
				true,
				elementCache.data.get(),
				resourcePtr->GetName()
			);

			cache.uploadBuffer->Initialize();
		}

		// Ensure upload buffer points to the current element data (in case element cache recreated the data)
		if (cache.uploadBuffer && cache.uploadBuffer->IsInitialized())
		{
			// If element data pointer has changed, update it on the upload buffer
			auto& elementCache = mElementCache->GetByIdNoInit(resourceId);
			cache.uploadBuffer->SetDataBuffer(elementCache.data.get());
		}

		// Update the buffer data for all frame resources to be safe
		cache.uploadBuffer->CopyAllData();
	}
}
