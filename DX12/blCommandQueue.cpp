#include <blCommandQueue.h>
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blCommandQueue::blCommandQueue(ComPtr<ID3D12Device8> device)
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		DX12_API_CALL(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
	}
}