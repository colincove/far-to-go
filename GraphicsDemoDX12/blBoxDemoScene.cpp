#include <blBoxDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blBoxScene::blBoxScene(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window)
		: blMeshDemoSceneBase(graphicsAPI, window)
	{
		auto mesh = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResourceWithName<StandardMeshResource>(L"Cube", Cube()));
		SetMeshResource(mesh);
	}
}