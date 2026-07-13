#pragma once

#include <blMeshLibrary.h>
#include <MeshDemoSceneBase.h>

namespace BoulderLeaf::Graphics
{
	class blGeosphereDemoScene : public blMeshDemoSceneBase
	{
	public:
		blGeosphereDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
	};
}
