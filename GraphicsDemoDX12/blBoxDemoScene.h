#pragma once

#include <blMeshLibrary.h>
#include <MeshDemoSceneBase.h>

namespace BoulderLeaf::Graphics
{
	class blBoxScene : public blMeshDemoSceneBase
	{
	public:
		blBoxScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
	};
}
