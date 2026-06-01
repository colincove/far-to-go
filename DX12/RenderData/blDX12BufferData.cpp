#include <blDX12BufferData.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12Buffer.h>
#include <Resources/blResourceManager.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12BufferDataCache::blDX12BufferDataCache(std::shared_ptr<blDevice> device)
		: mDevice(device), blDX12ResourceDataCache()
	{

	}

	void blDX12BufferDataCache::InitializeCache(
		const blStandardObjectConstantsBufferResource& resource,
		blDX12BufferData& cache)
	{
		using namespace DirectX;

		cache.dataBuffer = blBasicDataBuffer(
			reinterpret_cast<const blDataBufferInterface&>(resource.GetData()),
			DX12::DX12BufferAdapter::Get());

		cache.uploadBuffer = std::make_shared<blUploadBuffer>(
			mDevice.get()->GetDX12Device().Get(),
			static_cast<UINT>(cache.dataBuffer.GetElementSize()),
			static_cast<UINT>(resource.GetData().Count()),
			true);

		cache.uploadBuffer->CopyData(0, cache.dataBuffer.Get(0));
	}

	void blDX12BufferDataCache::UpdateCache(const blResourceId& resourceId)
	{
		//TODO: UpdateCache needs to take the actual resource data. In order to update the data. You dummy. 
		//cache.dataBuffer = blBasicDataBuffer(
		//	reinterpret_cast<const blDataBufferInterface&>(resource.GetData()),
		//	DX12::DX12BufferAdapter::Get());

		blDX12BufferData& cache = GetByIdNoInit(resourceId);

		const blResourcePtr<blStandardObjectConstantsBufferResource> resourcePtr = blResourceManager::Get()
			.GetResource<blStandardObjectConstantsBufferResource>(resourceId);

		cache.dataBuffer = blBasicDataBuffer(
			reinterpret_cast<const blDataBufferInterface&>(resourcePtr->GetData()),
			DX12::DX12BufferAdapter::Get());

		cache.uploadBuffer->CopyData(0, cache.dataBuffer.Get(0));
	}
}