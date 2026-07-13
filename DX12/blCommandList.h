#pragma once

#include <blDX12Core.h>
#include <blDevice.h>
#include <blCommandListAllocator.h>
#include <d3d12.h>
#include <memory>
#include <blPSO.h>

namespace
{
	#define COMMAND_LIST_FUNCTION_WRAPPER(FunctionName) \
	template<typename... Args> \
	void FunctionName(Args&&... args) \
	{ \
		mCommandList->FunctionName(std::forward<Args>(args)...); \
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandList
	{
	private:
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
		blCommandListAllocator* mCommandListAllocator;
	public:
		blCommandList(blCommandListAllocator* commandListAllocator, std::wstring name = L"");
		ComPtr<ID3D12GraphicsCommandList>& GetCommandListPtr() { return mCommandList; }
		
		void Close() const;
		void Reset(blCommandListAllocator* commandListAllocator);
		void Reset();

		COMMAND_LIST_FUNCTION_WRAPPER(RSSetViewports)
		COMMAND_LIST_FUNCTION_WRAPPER(RSSetScissorRects)
		COMMAND_LIST_FUNCTION_WRAPPER(ResourceBarrier)
		COMMAND_LIST_FUNCTION_WRAPPER(ClearRenderTargetView)
		COMMAND_LIST_FUNCTION_WRAPPER(ClearDepthStencilView)
		COMMAND_LIST_FUNCTION_WRAPPER(SetDescriptorHeaps)
		COMMAND_LIST_FUNCTION_WRAPPER(OMSetRenderTargets)
		COMMAND_LIST_FUNCTION_WRAPPER(SetGraphicsRootSignature)
		COMMAND_LIST_FUNCTION_WRAPPER(IASetVertexBuffers)
		COMMAND_LIST_FUNCTION_WRAPPER(IASetIndexBuffer)
		COMMAND_LIST_FUNCTION_WRAPPER(IASetPrimitiveTopology)
		COMMAND_LIST_FUNCTION_WRAPPER(SetPipelineState)
		COMMAND_LIST_FUNCTION_WRAPPER(SetGraphicsRootDescriptorTable)
		COMMAND_LIST_FUNCTION_WRAPPER(DrawIndexedInstanced)

	};
}