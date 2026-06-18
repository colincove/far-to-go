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

		const blStandardObjectConstantsBuffer& resourceData = resource.GetData();

		cache.dataBuffer = blBasicDataBuffer(
			reinterpret_cast<const blDataBufferInterface&>(resourceData),
			DX12::DX12BufferAdapter::Get(),
			256);

		cache.uploadBuffer = std::make_shared<blUploadBuffer>(
			mDevice.get()->GetDX12Device().Get(),
			static_cast<UINT>(cache.dataBuffer.GetElementSize()),
			static_cast<UINT>(resourceData.Count()),
			true);

		cache.uploadBuffer->Resource()->SetName(resourceData.GetName().c_str());
		cache.uploadBuffer->CopyInstanedData(0, (int) resourceData.Count(), cache.dataBuffer.Get(0));
	}

	void blDX12BufferDataCache::UpdateCache(const blResourceId& resourceId)
	{
		blDX12BufferData& cache = GetByIdNoInit(resourceId);

		const blStandardObjectConstantsBufferResourcePtr resourcePtr = blResourceManager::Get()
			.GetResource<blStandardObjectConstantsBufferResource>(resourceId);
		const blStandardObjectConstantsBuffer& resourceData = resourcePtr->GetData();

		const UINT enforceConstanceBufferStride = Math::CalcConstantBufferByteSize(
			GetBufferElementSize(resourceData.GetDataElementDescriptions(), DX12::DX12BufferAdapter::Get())
		);

		cache.dataBuffer = blBasicDataBuffer(
			reinterpret_cast<const blDataBufferInterface&>(resourceData),
			DX12::DX12BufferAdapter::Get(),
			enforceConstanceBufferStride);

		cache.uploadBuffer->CopyInstanedData(0, (int) resourceData.Count(), cache.dataBuffer.Get(0));
		//cache.uploadBuffer->CopyData(0, cache.dataBuffer.Get(0));
	}
}