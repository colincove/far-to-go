#include <blFence.h>
#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blFence::blFence(ComPtr<ID3D12Device8> device) 
	{
		DX12_API_CALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	}
}