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
		std::shared_ptr<blDevice> mDevice;
	public:
		blCommandQueue(std::shared_ptr<blDevice> device, std::wstring name = L"");

		ComPtr<ID3D12CommandQueue> GetDX12CommandQueue()
		{
			return mCommandQueue;
		}

		std::shared_ptr<blDevice> GetDevice()
		{
			return mDevice;
		}

		void ExecuteCommandLists(const std::vector<std::shared_ptr<blCommandList>>& commandLists) const
		{
			const UINT numCommandLists = (UINT) commandLists.size();

			std::vector<ID3D12CommandList*> commandListsVector;
			commandListsVector.reserve(numCommandLists);

			for (const std::shared_ptr<blCommandList>& commandList : commandLists)
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