#include <blDX12ConstantBufferCache.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ConstantBufferCache::blDX12ConstantBufferCache(
		std::shared_ptr<blDevice> device,
		std::shared_ptr<blDX12BufferDataCache> bufferCache,
		std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext) :
		mDevice(device), 
		mBufferCache(bufferCache),
		mGlobalRenderFrameContext(globalRenderFrameContext)
	{
	}

	//TODO: Why is this data specific to standard object constants? I want to be able to use this for any type of constant buffer.
	void blDX12ConstantBufferCache::InitializeCache(
		const blDataBufferInterfaceResource& resource,
		blDX12ConstantBufferCacheData& cache)
	{
		blDX12BufferData& bufferData = mBufferCache->Get(resource);

		cache.descriptorHeap = std::make_shared<blConstantBufferDescriptorHeap>(
			mDevice,
			(UINT) (resource.GetData().Count() * Constants::FrameResourceCount),
			resource.GetName());

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(bufferData.dataBuffer.GetElementSize()));
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = bufferData.uploadBuffer->Resource()->GetGPUVirtualAddress();
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cache.descriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

		const size_t count = resource.GetData().Count();
		for (int frameResource = 0; frameResource < Constants::FrameResourceCount; ++frameResource)
		{
			for (int i = 0; i < count; ++i)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

				//this is ugly. probably need to figure out a cleaner way to calculate and share
				cbvDesc.BufferLocation = (cbAddress + ((objCBByteSize * count) * frameResource)) + (i * objCBByteSize);
				cbvDesc.SizeInBytes = objCBByteSize;
				mDevice->GetDX12Device()->CreateConstantBufferView(
					&cbvDesc,
					cbvHandle);
				cbvHandle.Offset(mDevice->GetCbvSrvDescriptorSize());
			}
		}
	}
}