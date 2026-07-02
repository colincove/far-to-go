#pragma once

#include <blMeshLibrary.h>
#include <MeshDemoSceneBase.h>

namespace BoulderLeaf::Graphics
{
	class blCylinderDemoScene : public blMeshDemoSceneBase
	{
	public:
		blCylinderDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window, blResourceContainer* resourceContainer);
	};
}
