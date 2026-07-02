#include <blUploadBuffer.h>
#include <blDX12Buffer.h>


namespace
{
	using namespace BoulderLeaf::Graphics;

	static UINT CalculateVertexBufferElementSize(
		const blDataBufferInterface& dataBufferInterface,
		bool isConstantBuffer)
	{
		UINT elementByteSize = GetBufferElementSize(dataBufferInterface.GetDataElementDescriptions(), DX12::DX12BufferAdapter::Get());

		if (isConstantBuffer)
		{
			elementByteSize = DX12::Math::CalcConstantBufferByteSize(elementByteSize);
		}

		return elementByteSize;
	}


	static UINT CalculateVertexBufferSize(
		const blDataBufferInterface& dataBufferInterface,
		bool isConstantBuffer)
	{
		UINT elementByteSize = CalculateVertexBufferElementSize(dataBufferInterface, isConstantBuffer);
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		return elementByteSize * dataBufferInterface.Count();
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	blUploadBufferBase::blUploadBufferBase(
		std::shared_ptr<blDevice> device,
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

	blDX12blDataBufferUploadBuffer_Resource::blDX12blDataBufferUploadBuffer_Resource(
		std::shared_ptr<blDevice> device,
		bool isConstantBuffer,
		blResourceHandleOfType<blDataBufferInterface> handle)
		: mHandle(handle), 
		blUploadBufferBase(device, isConstantBuffer, std::wstring(handle.GetName()))
	{

	}

	UINT blDX12blDataBufferUploadBuffer_Resource::GetTotalSize() const
	{
		//TODO
		return 0;
	}

	void blDX12blDataBufferUploadBuffer_Resource::CopyData(int currentFrameResource)
	{
		//TODO
	}

	void blDX12blDataBufferUploadBuffer_Resource::CopyAllData()
	{
		//TODO
	}

	blDX12blVertexDataUploadBuffer::blDX12blVertexDataUploadBuffer(
		std::shared_ptr<blDevice> device,
		bool isConstantBuffer,
		blDataBufferInterfaceWeakPtr dataPtr,
		std::wstring name)
		: mDataPtr(dataPtr),
		blUploadBufferBase(device, isConstantBuffer, name)
	{

	}

	UINT blDX12blVertexDataUploadBuffer::GetTotalSize() const
	{
		blDataBufferInterfacePtr strongPtr = mDataPtr.lock();

		if (!strongPtr)
		{
			assert(false);
			return 0;
		}

		const UINT singleVertexBufferSize = CalculateVertexBufferSize(
			*strongPtr,
			mIsConstantBuffer);

		return singleVertexBufferSize;
	}

	D3D12_GPU_VIRTUAL_ADDRESS blDX12blVertexDataUploadBuffer::GetBufferLocationForIndex(int currentFrameResource, int index)
	{
		blDataBufferInterfacePtr strongPtr = mDataPtr.lock();

		if (!strongPtr)
		{
			assert(false);
			return D3D12_GPU_VIRTUAL_ADDRESS();
		}

		// ? cache ?
		// maybe create an iterator that can cache this data while it works?
		UINT elementByteSize = CalculateVertexBufferElementSize(*strongPtr, mIsConstantBuffer);
		return GetBufferLocationStart(currentFrameResource) + (index * elementByteSize);
	}

	void blDX12blVertexDataUploadBuffer::CopyAllData()
	{
		for (int i = 0; i < (mIsConstantBuffer ? Constants::FrameResourceCount : 1); ++i)
		{
			CopyData(i);
		}
	}

	void blDX12blVertexDataUploadBuffer::CopyData(int currentFrameResource)
	{
		assert(IsInitialized());
		assert(!(currentFrameResource > 0 && !mIsConstantBuffer));

		blDataBufferInterfacePtr strongPtr = mDataPtr.lock();

		if (!strongPtr)
		{
			assert(false);
			return;
		}

		const UINT totalSize = GetTotalSize();
		const UINT offset = mIsConstantBuffer ? (totalSize * currentFrameResource) : 1;

		memcpy(&mMappedData[0] + offset, &strongPtr->Get(0), totalSize);
	}

	blDX12VertexBufferWithPassUploadBuffer::blDX12VertexBufferWithPassUploadBuffer(
		std::shared_ptr<blDevice> device,
		bool isConstantBuffer,
		blVertexBufferWithPassBufferWeakPtr dataPtr,
		std::wstring name)
		: mDataPtr(dataPtr),
		blUploadBufferBase(device, isConstantBuffer, name)
	{
	}

	UINT blDX12VertexBufferWithPassUploadBuffer::GetTotalSize() const
	{
		blVertexBufferWithPassBufferPtr strongPtr = mDataPtr.lock();

		if (!strongPtr)
		{
			assert(false);
			return 0;
		}

		const blVertexBufferWithPassBuffer& resourceData = *strongPtr;
		UINT passElementSize = GetBufferElementSize(
			resourceData.GetPassElementBuffer()->GetData().GetElementDescriptions(), 
			DX12::DX12BufferAdapter::Get());

		if (mIsConstantBuffer)
		{
			passElementSize = DX12::Math::CalcConstantBufferByteSize(passElementSize);
		}

		const UINT vertexBufferSize = CalculateVertexBufferSize(
			resourceData.GetVertexBuffer()->GetData(),
			mIsConstantBuffer);

		const UINT totalSize = passElementSize + vertexBufferSize;
		return totalSize;
	}

	void blDX12VertexBufferWithPassUploadBuffer::CopyData(int currentFrameResource)
	{
		assert(IsInitialized());
		assert(!(currentFrameResource > 0 && !mIsConstantBuffer));

		blVertexBufferWithPassBufferPtr strongPtr = mDataPtr.lock();

		if (!strongPtr)
		{
			assert(false);
			return;
		}

		const UINT totalSize = GetTotalSize();
		UINT offset = 0;

		// this is so so bad. I need to see, again, if I can memcpy data in sections. 
		// instead of one large copy. For some reason, when I tried in the past, it did not work right?
		auto combinedDataBuffer = std::make_unique<byte[]>(totalSize);

		const blVertexBufferWithPassBuffer& resourceData = *strongPtr;
		const UINT passElementSize = GetBufferElementSize(
			resourceData.GetPassElementBuffer()->GetData().GetElementDescriptions(),
			DX12::DX12BufferAdapter::Get());

		const UINT vertexBufferSize = CalculateVertexBufferSize(
			resourceData.GetVertexBuffer()->GetData(),
			mIsConstantBuffer);

		memcpy(&combinedDataBuffer, resourceData.GetPassElementBuffer()->GetData().GetData(), passElementSize);
		memcpy(&combinedDataBuffer + passElementSize, &resourceData.GetVertexBuffer()->GetData().Get(0), vertexBufferSize);

		if (mIsConstantBuffer)
		{
			offset = totalSize * currentFrameResource;
		}

		memcpy(&mMappedData[0] + offset, &combinedDataBuffer, totalSize);
	}

	blDX12ElementUploadBuffer::blDX12ElementUploadBuffer(
		std::shared_ptr<blDevice> device,
		bool isConstantBuffer,
		blDataElementBuffer* dataPtr,
		std::wstring name)
		: mDataPtr(dataPtr),
		blUploadBufferBase(device, isConstantBuffer, name)
	{
	}

	UINT blDX12ElementUploadBuffer::GetTotalSize() const
	{
		UINT elementSize = GetBufferElementSize(
			mDataPtr->GetElementDescriptions(),
			DX12::DX12BufferAdapter::Get());

		if (mIsConstantBuffer)
		{
			elementSize = DX12::Math::CalcConstantBufferByteSize(elementSize);
		}

		return elementSize;
	}

	void blDX12ElementUploadBuffer::CopyData(int currentFrameResource)
	{
		assert(IsInitialized());
		assert(!(currentFrameResource > 0 && !mIsConstantBuffer));

		const UINT totalSize = GetTotalSize();
		const UINT offset = mIsConstantBuffer ? (totalSize * currentFrameResource) : 1;

		memcpy(&mMappedData[0] + offset, mDataPtr->GetData(), totalSize);
	}

	void blDX12ElementUploadBuffer::CopyAllData()
	{
		for (int i = 0; i < (mIsConstantBuffer ? Constants::FrameResourceCount : 1); ++i)
		{
			CopyData(i);
		}
	}
}
