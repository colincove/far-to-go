#include <blDX12BufferElementDataCache.h>
#include <blDX12Buffer.h>
#include <Resources/blResourceManager.h>
#include <blBuffer.h>
#include <blDX12Math.inl>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12BufferElementCache::blDX12BufferElementCache()
		: blDX12ResourceDataCache<blDX12BufferElementCacheData, blDataElementBufferResource>()
	{
	}

	void blDX12BufferElementCache::InitializeCache(
		const blDataElementBufferResource& resource,
		blDX12BufferElementCacheData& cache)
	{
		const blDataElementBuffer& resourceData = resource.GetData();

		const std::vector<BufferElementDescription>& elementDescriptions = resourceData.GetElementDescriptions();

		cache.elementSize = Math::CalcConstantBufferByteSize(
			GetBufferElementSize(elementDescriptions, DX12::DX12BufferAdapter::Get()));

		// Allocate storage for a single element (blDataElementBuffer is used for single-element pass data)
		std::unique_ptr<byte[]> destData = std::make_unique<byte[]>(cache.elementSize);

		// Marshal the single element from BoulderLeaf format into DX12 format
		MarshalBufferElement(elementDescriptions, destData.get(), resourceData.GetData(), DX12::DX12BufferAdapter::Get());

		cache.data = std::make_unique<blDataElementBuffer>(
			DX12::DX12BufferAdapter::Get().GetFormat(),
			elementDescriptions,
			std::move(destData)
		);
	}

	void blDX12BufferElementCache::UpdateCache(const blResourceId& resourceId)
	{
		blDX12BufferElementCacheData& cache = GetByIdNoInit(resourceId);

		const blDataElementBufferResourcePtr resourcePtr = blResourceManager::Get()
			.GetResource<blDataElementBufferResource>(resourceId);

		const blDataElementBuffer& resourceData = resourcePtr->GetData();

		const std::vector<BufferElementDescription>& elementDescriptions = resourceData.GetElementDescriptions();

		const size_t destElementSize = GetBufferElementSize(elementDescriptions, DX12::DX12BufferAdapter::Get());

		std::unique_ptr<byte[]> destData = std::make_unique<byte[]>(destElementSize);

		MarshalBufferElement(elementDescriptions, destData.get(), resourceData.GetData(), DX12::DX12BufferAdapter::Get());

		cache.data = std::make_unique<blDataElementBuffer>(
			DX12::DX12BufferAdapter::Get().GetFormat(),
			elementDescriptions,
			std::move(destData)
		);
	}
}

