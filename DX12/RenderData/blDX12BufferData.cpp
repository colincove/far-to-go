#include <blDX12BufferData.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12Buffer.h>
#include <Resources/blResourceManager.h>
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12BufferDataCache::blDX12BufferDataCache(
		std::shared_ptr<blDevice> device, 
		std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext)
		: mDevice(device), blDX12ResourceDataCache(), mGlobalRenderFrameContext(globalRenderFrameContext)
	{

	}

	void blDX12BufferDataCache::InitializeCache(
		const blDataBufferInterfaceResource& resource,
		blDX12BufferData& cache)
	{
		using namespace DirectX;

		const blDataBufferInterface& resourceData = resource.GetData();
		const UINT enforceConstanceBufferStride = Math::CalcConstantBufferByteSize(
			(UINT) GetBufferElementSize(resourceData.GetDataElementDescriptions(), DX12::DX12BufferAdapter::Get())
		);

		cache.dataBuffer = std::make_shared<blBasicDataBuffer>(
			reinterpret_cast<const blDataBufferInterface&>(resourceData),
			DX12::DX12BufferAdapter::Get(),
			enforceConstanceBufferStride);

		cache.uploadBuffer = std::make_unique<blDX12blVertexDataUploadBuffer>(
			mDevice,
			true,
			cache.dataBuffer,
			resource.GetName()
		);

		cache.uploadBuffer->Initialize();
		cache.uploadBuffer->CopyData(mGlobalRenderFrameContext->GetCurrentFrameResource());
	}

	void blDX12BufferDataCache::UpdateCache(const blResourceId& resourceId)
	{
		blDX12BufferData& cache = GetByIdNoInit(resourceId);

		const blDataBufferInterfaceResourcePtr resourcePtr = blResourceManager::Get()
			.GetResource<blDataBufferInterfaceResource>(resourceId);
		const blDataBufferInterface& resourceData = resourcePtr->GetData();

		const UINT enforceConstanceBufferStride = Math::CalcConstantBufferByteSize(
			(UINT) GetBufferElementSize(resourceData.GetDataElementDescriptions(), DX12::DX12BufferAdapter::Get())
		);

		cache.dataBuffer = std::make_shared<blBasicDataBuffer>(
			reinterpret_cast<const blDataBufferInterface&>(resourceData),
			DX12::DX12BufferAdapter::Get(),
			enforceConstanceBufferStride);

		
		cache.uploadBuffer->SetDataBuffer(cache.dataBuffer);

		// OPTOMIZATION NEEDED
		// we are updating every frame resource. This is to avoid issues where the update is not
		// updated every frame, and the next frame resource was not updated. 
		// we should only update mGlobalRenderFrameContext->GetCurrentFrameResource(). 
		// but we need a way to update the frame resource next frame if there was no change made from outside.
		cache.uploadBuffer->CopyAllData();
	}
}