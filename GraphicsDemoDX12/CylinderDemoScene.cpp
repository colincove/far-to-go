#include <CylinderDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blCylinderDemoScene::blCylinderDemoScene(
		API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blMeshDemoSceneBase(graphicsAPI, window, resourceContainer)
	{
		SetMeshAndShaderData(CreateCylinderResource(resourceContainer,
			2,    //bottomRadius
			1,    //topRadius
			3,    //height
			20,   //sliceCount
			10    //stackCount
		));
	}
}