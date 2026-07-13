#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include <memory>
#include <blDevice.h>
#include <blCommandList.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blCommandQueue
	{
	private:
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		blDevice* mDevice;
	public:
		blCommandQueue(blDevice* device, std::wstring name = L"");

		ComPtr<ID3D12CommandQueue> GetDX12CommandQueue()
		{
			return mCommandQueue;
		}

		blDevice* GetDevice()
		{
			return mDevice;
		}

		void ExecuteCommandLists(std::vector<blCommandList*> commandLists) const
		{
			const UINT numCommandLists = (UINT) commandLists.size();

			std::vector<ID3D12CommandList*> commandListsVector;
			commandListsVector.reserve(numCommandLists);

			for (blCommandList* commandList : commandLists)
			{
				commandListsVector.push_back(commandList->GetCommandListPtr().Get());
			}

			mCommandQueue->ExecuteCommandLists(numCommandLists, commandListsVector.data());
		}

		template<typename... Args>
		void Signal(Args&&... args)
		{
			mCommandQueue->Signal(std::forward<Args>(args)...);
		}
	};
}