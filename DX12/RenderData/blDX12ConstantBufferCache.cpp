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

	//TODO: Why is this data specific to standard object constants? I want to be able to use this for any type of constant buffer.
	void blDX12ConstantBufferCache::InitializeCache(
		const blStandardObjectConstantsBufferResource& resource,
		blDX12ConstantBufferCacheData& cache)
	{
		blDX12BufferData& bufferData = mBufferCache->Get(resource);

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(bufferData.dataBuffer.GetElementSize()));
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = bufferData.uploadBuffer->Resource()->GetGPUVirtualAddress();
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

		for (int i = 0; i < resource.GetData().Count(); ++i)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress + i * objCBByteSize;
			cbvDesc.SizeInBytes = objCBByteSize;
			mDevice->GetDX12Device()->CreateConstantBufferView(
				&cbvDesc,
				cbvHandle);
			cbvHandle.Offset(mDevice->GetCbvSrvDescriptorSize());

		}
	}
}