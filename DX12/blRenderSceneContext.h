#pragma once

#include <blRenderScene.h>
#include <blDevice.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blRenderSceneContext
	{
	public:
		blRenderSceneContext() = default;
		blRenderSceneContext(std::shared_ptr<blDevice> device);
	};
}