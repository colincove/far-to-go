#pragma once

#include <memory>
#include <blDX12UploadBuffer.h>
#include <blBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blConstantBuffer
	{
	private:
		//std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	public:
		blConstantBuffer(
			std::shared_ptr<blDevice> mDevice,
			std::shared_ptr<blDataBufferInterface> DataBuffer
			);
	};
}