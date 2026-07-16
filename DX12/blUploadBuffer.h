#pragma once

#include <windows.h>
#include <blDX12Math.inl>
#include <blDevice.h>
#include "../DirectX-Headers/include/directx/d3dx12.h"
#include <blBuffer.h>
#include <Resources/blResources.h>
#include <Resources/blResourceHandleOfType.h>


namespace BoulderLeaf::Graphics::DX12
{
	class blUploadBufferBase
	{
	private:
		blDevice* mDevice;
		std::wstring mName;
		ComPtr<ID3D12Resource> mUploadBuffer;
		bool mIsInitialized;
	protected:
		blUploadBufferBase(
			blDevice* device,
			bool isConstantBuffer,
			std::wstring name);

		virtual UINT GetTotalSize() const = 0;

		bool mIsConstantBuffer;
		BYTE* mMappedData = nullptr;

	public:
		virtual void CopyData(int currentFrameResource) = 0;
		virtual void CopyAllData() = 0;
		bool IsConstantBuffer() const { return mIsConstantBuffer; }
		bool IsInitialized() { return mIsInitialized; }
		void Initialize();

		ID3D12Resource* Resource()const
		{
			return mUploadBuffer.Get();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetBufferLocationStart(int currentFrameResource);
	};

	class blDX12blDataBufferUploadBuffer_Resource final : public blUploadBufferBase
	{
	private:
		blResourceHandleOfType<blDataBufferInterface> mHandle;
	protected:
		blDX12blDataBufferUploadBuffer_Resource(
			blDevice* device,
			bool isConstantBuffer,
			blResourceHandleOfType<blDataBufferInterface> handle
		);

		virtual UINT GetTotalSize() const;
		virtual void CopyData(int currentFrameResource);
		virtual void CopyAllData();
	};

	class blDX12ListResourceUploadBuffer final : public blUploadBufferBase
	{
	private:
		blResourceHandleOfType<blListResource> mData;
	public:
		blDX12ListResourceUploadBuffer(
			blDevice* device,
			bool isConstantBuffer,
			blResourceHandleOfType<blListResource> data,
			std::wstring name
		);

		virtual void CopyData(int currentFrameResource) override;
		virtual void CopyAllData() override;
		D3D12_GPU_VIRTUAL_ADDRESS GetBufferLocationForIndex(int currentFrameResource, int index);
	protected:
		virtual UINT GetTotalSize() const override;
	};
}
