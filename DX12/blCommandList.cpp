#include "blCommandList.h"
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandList::blCommandList(std::shared_ptr<blCommandListAllocator> commandListAllocator, std::wstring name)
		: mCommandListAllocator(commandListAllocator)
	{
		HRESULT result = commandListAllocator->GetDevice()->GetDX12Device()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList));

		// NOTE: Name not showing up in PIX
		/*Although the DX12 backend sets the debug name of CommandBuffers by calling SetName on the underlying GraphicsCommandList, it doesn't show up in the tested graphics debuggers (RenderDoc, PIX). Baldur Karlsson (RenderDoc author) explained why:
		command buffers are inherently transient, and the lifetime of when exactly the name should be applied is a little unclear. They are better annotated using markers rather than names, if you want to have a global name for the command buffer you could insert it as a marker as the first command
		Markers are already set by the DX12 backend to mark bound descriptor sets, so the initial pieces are there if we want to take this route.*/
		mCommandList->SetName((L"[BL] " + name).c_str());

		// TODO: All of this testing was some debugging. It makes me realize that my DX12_API_CALL is not good. 
		// The application cannot validate the return result. And all errors are just tossed out with the assert. 
		// I need a better way to process the result. Asset on it. But also respond to errors, if I want to. 
		int test = 0;
		switch (result)
		{
			case S_OK:
				test = 43;
				break;
			case D3D12_ERROR_ADAPTER_NOT_FOUND:
				test = 74;
				break;
			case D3D12_ERROR_DRIVER_VERSION_MISMATCH:
				test = 65;
				break;
			case E_OUTOFMEMORY:
				test = 96;

				break;
			case E_INVALIDARG:
				test = 78;
				break;
			case E_FAIL:
				test = 86;
				break;
			case E_NOTIMPL:
				test = 32;
				break;
			case E_POINTER:
				test = 97;
				break;
		}

		assert(result == S_OK);

		/*DX12_API_CALL(commandListAllocator->GetDevice()->GetDX12Device()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			commandListAllocator->GetAllocatorPtr().Get(),
			nullptr, //initial pipeline state
			IID_PPV_ARGS(&mCommandList)));*/
	}
}