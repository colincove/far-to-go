#include <CylinderDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blCylinderDemoScene::blCylinderDemoScene(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window)
		: blMeshDemoSceneBase(graphicsAPI, window)
	{
		auto mesh = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(CreateCylinder(
				2,    //bottomRadius
				1,    //topRadius
				3,    //height
				20,   //sliceCount
				10    //stackCount
			)));
		SetMeshResource(mesh);
	}
}