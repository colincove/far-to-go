#pragma once

#include <blMaterial.h>
#include <blDevice.h>
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class blPipelineState
	{
	private:
		std::shared_ptr<blDevice> mDevice;
	public:
		blPipelineState(std::shared_ptr<blDevice> device);
		void SetState(blMaterial& material);
	};
}