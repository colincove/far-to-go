#pragma once

#include <blMeshLibrary.h>
#include <MeshDemoSceneBase.h>

namespace BoulderLeaf::Graphics
{
	class blGeosphereDemoScene : public blMeshDemoSceneBase
	{
	public:
		blGeosphereDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
	};
}
