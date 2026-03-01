#include <blDX12ConstantBufferCache.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ConstantBufferCache::blDX12ConstantBufferCache(
		std::shared_ptr<blDevice> device,
		std::shared_ptr<blConstantBufferDescriptorHeap> cbvHeap,
		std::shared_ptr<blDX12BufferDataCache> bufferCache) : 
		mDevice(device), 
		mBufferCache(bufferCache),
		mCbvHeap(cbvHeap)
	{
	}

	void blDX12ConstantBufferCache::InitializeCache(
		const blDataBufferInterfaceResource& resource,
		blDX12ConstantBufferCacheData& cache)
	{
		blDX12BufferData& bufferData = mBufferCache->Get(resource);

		UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(bufferData.dataBuffer.GetElementSize()));
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = bufferData.uploadBuffer->mUploadBuffer->GetGPUVirtualAddress();
		// Offset to the ith object constant buffer in the buffer.
		// Here our i = 0.
		int boxCBufIndex = 0;
		cbAddress += boxCBufIndex * objCBByteSize;
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = Math::CalcConstantBufferByteSize(static_cast<UINT>(bufferData.dataBuffer.GetElementSize()));
		mDevice->GetDX12Device()->CreateConstantBufferView(
			&cbvDesc,
			mCbvHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

	}
}