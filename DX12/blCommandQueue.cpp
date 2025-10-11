#include <blCommandQueue.h>
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandQueue::blCommandQueue(std::shared_ptr<blDevice> device)
		: mDevice(device)
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		DX12_API_CALL(device->GetDX12Device()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	}
}