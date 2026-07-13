#include <blBoxDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blBoxScene::blBoxScene(
		API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blMeshDemoSceneBase(graphicsAPI, window, resourceContainer)
	{
		SetMeshResourceExprimentalFromInlineMesh(L"Cube", CubeInline(),
			BufferDescription{
				BufferFormat::BoulderLeaf,
				StandardVertexDefinition::Description
			}
		);

		auto mesh = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(Cube()));
		SetMeshResource(mesh);
	}
}