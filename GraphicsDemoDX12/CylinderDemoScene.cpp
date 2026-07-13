#include <CylinderDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blCylinderDemoScene::blCylinderDemoScene(
		API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blMeshDemoSceneBase(graphicsAPI, window, resourceContainer)
	{

		std::unique_ptr<byte[]> cubeMesh = CubeInline();
		SetMeshResourceExprimentalFromInlineMesh(L"Cube", CubeInline(),
			BufferDescription{
				BufferFormat::BoulderLeaf,
				StandardVertexDefinition::Description
			});

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