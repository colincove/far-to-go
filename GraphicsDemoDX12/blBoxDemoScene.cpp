#include <blBoxDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blBoxScene::blBoxScene(
		API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blMeshDemoSceneBase(graphicsAPI, window, resourceContainer)
	{
		SetMeshAndShaderData(CubeResource(resourceContainer));
	}
}