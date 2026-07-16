#include <blUploadBuffer.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blUploadBufferBase::blUploadBufferBase(
		blDevice* device,
		bool isConstantBuffer,
		std::wstring name)
		:mDevice(device),
		mIsConstantBuffer(isConstantBuffer),
		mName(name),
		mIsInitialized(false)
	{
	}
	void blUploadBufferBase::Initialize()
	{
		assert(!mIsInitialized);
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		const UINT size = GetTotalSize() * (mIsConstantBuffer ? Constants::FrameResourceCount : 1);

		auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

		mDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer));

		mUploadBuffer->SetName(mName.c_str());
		mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
		// We do not need to unmap until we are done with the resource.
		// However, we must not write to the resource while it is in use by
		// the GPU (so we must use synchronization techniques).

		mIsInitialized = true;
	}

	D3D12_GPU_VIRTUAL_ADDRESS blUploadBufferBase::GetBufferLocationStart(int currentFrameResource)
	{
		assert(IsInitialized());
		assert(!(currentFrameResource > 0 && !mIsConstantBuffer));

		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mUploadBuffer->GetGPUVirtualAddress()
			+ GetTotalSize() * currentFrameResource;

		return cbAddress;
	}

	blDX12ListResourceUploadBuffer::blDX12ListResourceUploadBuffer(
			blDevice* device,
			bool isConstantBuffer,
			blResourceHandleOfType<blListResource> data,
			std::wstring name)
		: mData(data),
		blUploadBufferBase(device, isConstantBuffer, name)
	{

	}

	UINT blDX12ListResourceUploadBuffer::GetTotalSize() const
	{
		return mData->mCount * mData->mElementSize;
	}

	D3D12_GPU_VIRTUAL_ADDRESS blDX12ListResourceUploadBuffer::GetBufferLocationForIndex(int currentFrameResource, int index)
	{
		return GetBufferLocationStart(currentFrameResource) + (index * mData->mElementSize);
	}

	void blDX12ListResourceUploadBuffer::CopyAllData()
	{
		for (int i = 0; i < (mIsConstantBuffer ? Constants::FrameResourceCount : 1); ++i)
		{
			CopyData(i);
		}
	}

	void blDX12ListResourceUploadBuffer::CopyData(int currentFrameResource)
	{
		assert(IsInitialized());
		assert(!(currentFrameResource > 0 && !mIsConstantBuffer));

		const UINT totalSize = GetTotalSize();
		const UINT offset = mIsConstantBuffer ? (totalSize * currentFrameResource) : 1;

		memcpy(&mMappedData[0] + offset, &mData->Get<byte>(0), totalSize);
	}
}
