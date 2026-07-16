#pragma once

#include <blVirtualCamera.h>
#include <blTypes.h>
#include <streambuf>
#include <vector>
#include <memory>
#include <Resources/blResources.h>

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

	struct blSceneResource : public blBaseResource
	{
		blSceneData mData;
	};
}