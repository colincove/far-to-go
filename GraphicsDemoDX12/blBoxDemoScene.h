#pragma once

#include <blMeshLibrary.h>
#include <MeshDemoSceneBase.h>

namespace BoulderLeaf::Graphics
{
	class blBoxScene : public blMeshDemoSceneBase
	{
	public:
		blBoxScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
	};
}
