#pragma once

#include <windows.h>
#include <blDX12Math.inl>
#include <blDevice.h>
#include "../DirectX-Headers/include/directx/d3dx12.h"
#include <blBuffer.h>
#include <Resources/blResourcesExprimental.h>
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

	class blDX12blVertexDataUploadBuffer final: public blUploadBufferBase
	{
	private:
		blDataBufferInterfaceWeakPtr mDataPtr;
	public:
		blDX12blVertexDataUploadBuffer(
			blDevice* device,
			bool isConstantBuffer,
			blDataBufferInterfaceWeakPtr dataPtr,
			std::wstring name
		);

		virtual void CopyData(int currentFrameResource) override;
		virtual void CopyAllData() override;
		void SetDataBuffer(blDataBufferInterfaceWeakPtr dataPtr)
		{
			mDataPtr = dataPtr;
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetBufferLocationForIndex(int currentFrameResource, int index);
	protected:
		virtual UINT GetTotalSize() const override;
	};

	class blDX12VertexBufferWithPassUploadBuffer final : public blUploadBufferBase
	{
	private:
		blVertexBufferWithPassBufferWeakPtr mDataPtr;
	public:
		blDX12VertexBufferWithPassUploadBuffer(
			blDevice* device,
			bool isConstantBuffer,
			blVertexBufferWithPassBufferWeakPtr dataPtr,
			std::wstring name
		);

		virtual void CopyData(int currentFrameResource) override;
	protected:
		virtual UINT GetTotalSize() const override;
	};

	class blDX12ElementUploadBuffer final : public blUploadBufferBase
	{
	private:
		blDataElementBuffer* mDataPtr;
	public:
		blDX12ElementUploadBuffer(
			blDevice* device,
			bool isConstantBuffer,
			blDataElementBuffer* dataPtr,
			std::wstring name
		);

		virtual void CopyData(int currentFrameResource) override;
		virtual void CopyAllData() override;
		void SetDataBuffer(blDataElementBuffer* dataPtr)
		{
			mDataPtr = dataPtr;
		}
	protected:
		virtual UINT GetTotalSize() const override;
	};
}
