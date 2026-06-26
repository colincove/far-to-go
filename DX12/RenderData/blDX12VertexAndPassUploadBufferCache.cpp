#include <blDX12VertexAndPassUploadBufferCache.h>
#include <blDX12Buffer.h>
#include <Resources/blResourceManager.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12VertexAndPassUploadBufferCache::blDX12VertexAndPassUploadBufferCache(std::shared_ptr<blDevice> device,
		std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext)
		: blDX12ResourceDataCache<blDX12VertexAndPassUploadBufferCacheData, blVertexBufferWithPassBufferResource>()
	{
	}

	void blDX12VertexAndPassUploadBufferCache::InitializeCache(
		const blVertexBufferWithPassBufferResource& resource,
		blDX12VertexAndPassUploadBufferCacheData& cache)
	{
		
	}

	void blDX12VertexAndPassUploadBufferCache::UpdateCache(const blResourceId& resourceId)
	{
		
	}
}

