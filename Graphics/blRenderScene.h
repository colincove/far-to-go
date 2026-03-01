#pragma once

#include <blVirtualCamera.h>
#include <blTypes.h>
#include <streambuf>
#include <vector>
#include <memory>
#include <Resources/blResource.h>

namespace BoulderLeaf::Graphics
{
	struct blSceneData
	{
		VirtualCamera m_camera;
	};

	class blScene
	{
	public:
		blScene() = default;
		~blScene() = default;
	private:
		blSceneData mData;
	};
}

BL_RESOURCE(blScene, eResourceType::Scene);