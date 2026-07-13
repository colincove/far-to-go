#include <blDX12ConstantBufferWithPassCache.h>
#include <blDX12Buffer.h>
#include <blDX12ElementUploadBufferCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ConstantBufferWithPassCache::blDX12ConstantBufferWithPassCache(
		blDevice* device,
		blDX12BufferDataCache* bufferCache,
		blDX12BufferElementCache* bufferElementCache,
		blGlobalRenderFrameContext* globalRenderFrameContext,
		blDX12ElementUploadBufferCache* elementUploadBufferCache) :
		mDevice(device),
		mBufferCache(bufferCache),
		mBufferElementCache(bufferElementCache),
		mGlobalRenderFrameContext(globalRenderFrameContext),
		mElementUploadBufferCache(elementUploadBufferCache)
	{
	}

	//TODO: Why is this data specific to standard object constants? I want to be able to use this for any type of constant buffer.
	void blDX12ConstantBufferWithPassCache::InitializeCache(
		const blVertexBufferWithPassBufferResource& resource,
		blDX12ConstantBufferCacheData& cache)
	{
		blDataBufferInterfaceResourcePtr vertexBufferPtr =
			std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(resource.GetData().GetVertexBuffer());
		
		//TODO: This is not a good upload buffer. It does not include the pass constants. 
		//I need to implement a different cache that will include pass constant data
		blDX12BufferData& bufferData = mBufferCache->Get(
			*vertexBufferPtr
		);

		blDX12ElementUploadBufferCacheData& passUploadBufferCache = mElementUploadBufferCache->Get(
			*resource.GetData().GetPassElementBuffer()
		);

		const blDX12BufferElementCacheData& passConstantData = mBufferElementCache->Get(*resource.GetData().GetPassElementBuffer());

		cache.descriptorHeap = std::make_unique<blConstantBufferDescriptorHeap>(
			mDevice,
			// +1 because of the pass constants
			(UINT)((vertexBufferPtr->GetData().Count() + 1) * Constants::FrameResourceCount),
			resource.GetName());

		const UINT objCBByteSize = Math::CalcConstantBufferByteSize(static_cast<UINT>(bufferData.dataBuffer->GetElementSize()));
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = bufferData.uploadBuffer->Resource()->GetGPUVirtualAddress();
		auto cbvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(cache.descriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
		
		const size_t count = vertexBufferPtr->GetData().Count();
		for (int frameResource = 0; frameResource < Constants::FrameResourceCount; ++frameResource)
		{
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

			//this is ugly. probably need to figure out a cleaner way to calculate and share
			//cbvDesc.BufferLocation = (cbAddress + ((objCBByteSize * count) * frameResource));
			cbvDesc.BufferLocation = passUploadBufferCache.uploadBuffer->GetBufferLocationStart(frameResource);
			//TODO: this size is wrong. needs to be the size of the pass constants
			cbvDesc.SizeInBytes = passConstantData.elementSize;
			mDevice->GetDX12Device()->CreateConstantBufferView(
				&cbvDesc,
				cbvHandle);

			cbvHandle.Offset(mDevice->GetCbvSrvDescriptorSize());

			for (int i = 0; i < count; ++i)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

				//this is ugly. probably need to figure out a cleaner way to calculate and share
				// TODO: Need to add offset for pass constants. Need to get the size of it. offset into the buffer
				//TODODODODOD: THis code below assumes that there is a single constant buffer resource holding 
				//both the pass data and the per object data. Right now, I am trying to use different resources. 
				//I think this will work....
				/*cbvDesc.BufferLocation = (cbAddress + //Start of buffer
					((objCBByteSize * count) * frameResource) + passConstantData.elementSize) + //offset to the frame resources + pass data
					(i * objCBByteSize); //offset to the item*/
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