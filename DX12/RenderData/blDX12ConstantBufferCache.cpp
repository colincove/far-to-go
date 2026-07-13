#include <blDX12ConstantBufferCache.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ConstantBufferCache::blDX12ConstantBufferCache(
		blDevice* device,
		blDX12BufferDataCache* bufferCache,
		blGlobalRenderFrameContext* globalRenderFrameContext) :
		mDevice(device), 
		mBufferCache(bufferCache),
		mGlobalRenderFrameContext(globalRenderFrameContext)
	{
	}

	void blDX12ConstantBufferCache::InitializeCache(
		const blDataBufferInterfaceResource& resource,
		blDX12ConstantBufferCacheData& cache)
	{
		blDX12BufferData& bufferData = mBufferCache->Get(resource);

		cache.descriptorHeap = std::make_unique<blConstantBufferDescriptorHeap>(
			mDevice,
			(UINT) (resource.GetData().Count() * Constants::FrameResourceCount),
			resource.GetName());

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(bufferData.dataBuffer->GetElementSize()));
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cache.descriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

		const size_t count = resource.GetData().Count();
		for (int frameResource = 0; frameResource < Constants::FrameResourceCount; ++frameResource)
		{
			for (int i = 0; i < count; ++i)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = bufferData.uploadBuffer->GetBufferLocationForIndex(frameResource, i);
				cbvDesc.SizeInBytes = objCBByteSize;
				mDevice->GetDX12Device()->CreateConstantBufferView(
					&cbvDesc,
					cbvHandle);
				cbvHandle.Offset(mDevice->GetCbvSrvDescriptorSize());
			}
		}
	}
}