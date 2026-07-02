#include <GeosphereDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blGeosphereDemoScene::blGeosphereDemoScene(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window, blResourceContainer* resourceContainer)
		: blMeshDemoSceneBase(graphicsAPI, window, resourceContainer)
	{
		SetMeshResourceExprimentalFromInlineMesh(L"Cube", CubeInline(),
			BufferDescription{
				BufferFormat::BoulderLeaf,
				StandardVertexDefinition::Description
			});

		auto mesh = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(CreateGeosphere(
				2,    //radius
				1    //numSubdivisions
			)));
		SetMeshResource(mesh);
	}
}