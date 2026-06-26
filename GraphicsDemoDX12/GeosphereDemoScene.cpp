#include <GeosphereDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blGeosphereDemoScene::blGeosphereDemoScene(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window)
		: blMeshDemoSceneBase(graphicsAPI, window)
	{
		auto mesh = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(CreateGeosphere(
				2,    //radius
				1    //numSubdivisions
			)));
		SetMeshResource(mesh);
	}
}