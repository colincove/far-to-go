#pragma once

#include <memory>
#include <blDX12ResourceDataCache.h>
#include <blBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12BufferElementCacheData
	{
		std::unique_ptr<blDataElementBuffer> data;

	};

	class blDX12BufferElementCache : public blDX12ResourceDataCache<blDX12BufferElementCacheData, blDataElementBufferResource>
	{
	public:
		blDX12BufferElementCache();
		virtual void UpdateCache(const blResourceId& resourceId) override;
	protected:
		virtual void InitializeCache(
			const blDataElementBufferResource& resource,
			blDX12BufferElementCacheData& cache) override;
	};
}