#pragma once

#include <DemoScene.h>
#include <Resources/blResourceHandleOfType.h>
#include <RenderItems/blRenderItemMesh.h>
#include <blVirtualCamera.h>
#include <blDemoImguiDropdown.h>

namespace BoulderLeaf
{
	class Metrics::blTime;
	class blResourceContainer;

	template<typename T>
	class blResourceHandleOfType;

	namespace Core
	{
		class blWindow;
	}

	namespace Graphics
	{
		class API;
		struct blIndexedMeshResource;
		struct blSceneResource;
	}
}

namespace BoulderLeaf::Graphics
{
	class blMaterialAndLightingDemoScene : public blDemoScene
	{
	private:
		blResourceHandleOfType<blIndexedMeshResource> mGeosphereMesh;
		blResourceHandleOfType<blArrayBufferResource> mObjectConstantBufferResource;
		blResourceHandleOfType<blListResource> mObjectConstantBufferListResource;
		blResourceHandleOfType<blArrayBufferResource> mPassConstantBufferResource;
		blResourceHandleOfType<blListResource> mPassConstantBufferListResource;

		blResourceHandleOfType<blSceneResource> mSceneResource;
		blResourceHandleOfType<blMaterialResource> mMaterialResource;
		std::vector<blResourceHandleOfType<blMaterialResource>> mMaterials;

		BoulderLeaf::Imgui::blDemoImguiDropdown mMaterialDropdown;

		RenderMeshDataInstancedV2 mDrawData;
		VirtualCamera mCamera;

		void SetMeshAndShaderData(blResourceHandleOfType<blIndexedMeshResource> mesh);
	public:
		blMaterialAndLightingDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
		virtual void Draw() override;
		virtual void Update(const Metrics::blTime& gameTime) override;
	};
}

