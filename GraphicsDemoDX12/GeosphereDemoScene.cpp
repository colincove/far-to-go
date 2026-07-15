#include <GeosphereDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blGeosphereDemoScene::blGeosphereDemoScene(
		API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blMeshDemoSceneBase(graphicsAPI, window, resourceContainer)
	{
		SetMeshAndShaderData(CreateGeosphereResource(resourceContainer,
			2,    //radius
			1    //numSubdivisions
		));
	}
}