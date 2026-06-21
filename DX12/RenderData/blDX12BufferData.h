#pragma once

#include <vector>
#include <memory>
#include <blBuffer.h>
#include <blDevice.h>
#include <blDX12ResourceDataCache.h>
#include <blDX12Math.inl>
#include "../DirectX-Headers/include/directx/d3dx12.h"
#include <blBufferTypes.h>
#include <blGlobalRenderFrameContext.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blUploadBuffer
	{
	public:
		blUploadBuffer(
			ID3D12Device* device, 
			UINT elementByteSize, 
			UINT elementCount, 
			bool isConstantBuffer)
			:mElementByteSize(elementByteSize),
			mIsConstantBuffer(isConstantBuffer)
		{
			// Constant buffer elements need to be multiples of 256 bytes.
			// This is because the hardware can only view constant data
			// at m*256 byte offsets and of n*256 byte lengths.
			// typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
			// UINT64 OffsetInBytes; // multiple of 256
			// UINT SizeInBytes; // multiple of 256
			// } D3D12_CONSTANT_BUFFER_VIEW_DESC;
			mConstantBufferElementByteSize = Math::CalcConstantBufferByteSize(mElementByteSize);
			const UINT elementBySizeToUse = isConstantBuffer ? mConstantBufferElementByteSize : elementByteSize;

			auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

			auto size = elementBySizeToUse * elementCount
				* (isConstantBuffer ? Constants::FrameResourceCount : 1);

			auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);

			device->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mUploadBuffer));

			mUploadBuffer->SetName(L"[BL] Upload Buffer");

			mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
			// We do not need to unmap until we are done with the resource.
			// However, we must not write to the resource while it is in use by
			// the GPU (so we must use synchronization techniques).
		}

		blUploadBuffer(const blUploadBuffer& rhs) = delete;
		blUploadBuffer& operator=(const blUploadBuffer& rhs) = delete;
		~blUploadBuffer()
		{
			if (mUploadBuffer != nullptr)
			{
				mUploadBuffer->Unmap(0, nullptr);
			}
			mMappedData = nullptr;
		}
		ID3D12Resource* Resource()const
		{
			return mUploadBuffer.Get();
		}
		void CopyData(int elementIndex, const byte& data)
		{
			memcpy(&mMappedData[elementIndex * mElementByteSize], &data, mElementByteSize);
		}

		void CopyInstanedData(int startIndex, int numElements, const byte& data, const int currentFrameResource)
		{
			const UINT elementBySizeToUse = mIsConstantBuffer ? mConstantBufferElementByteSize : mElementByteSize;
			auto offset = (currentFrameResource * (numElements * elementBySizeToUse) * mIsConstantBuffer);
			memcpy(&mMappedData[0] + offset, &data, numElements * elementBySizeToUse);
		}

		ComPtr<ID3D12Resource> mUploadBuffer;
		BYTE* mMappedData = nullptr;
		UINT mElementByteSize = 0;
		UINT mConstantBufferElementByteSize = 0;
		bool mIsConstantBuffer = false;		
	};

	struct blDX12BufferData
	{
		blBasicDataBuffer dataBuffer;
		std::shared_ptr<blUploadBuffer> uploadBuffer;
	};

	class blDX12BufferDataCache : 
		//public blDX12ResourceDataCache<blDX12BufferData, blDataBufferInterfaceResource>
		public blDX12ResourceDataCache<blDX12BufferData, blStandardObjectConstantsBufferResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blGlobalRenderFrameContext> mGlobalRenderFrameContext;
	public:
		blDX12BufferDataCache(std::shared_ptr<blDevice> device, 
			std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext);
		virtual void UpdateCache(const blResourceId& resourceId) override;
	protected:
		virtual void InitializeCache(
			const blStandardObjectConstantsBufferResource& resource,
			blDX12BufferData& cache) override;
	};
}