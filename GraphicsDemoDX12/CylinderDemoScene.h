#pragma once

#include <blMeshLibrary.h>
#include <MeshDemoSceneBase.h>

namespace BoulderLeaf::Graphics
{
	class blCylinderDemoScene : public blMeshDemoSceneBase
	{
	public:
		blCylinderDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
	};
}
