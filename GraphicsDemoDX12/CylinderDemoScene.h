#pragma once

#include <blResource.h>
#include <blShader.h>
#include <blMesh.h>
#include <blGraphicsAPI.h>
#include <blRenderScene.h>
#include <blMeshLibrary.h>
#include <blMaterial.h>
#include <blBufferTypes.h>
#include <blVirtualCamera.h>
#include <blWindow.h>
#include <blTime.h>
#include <DemoScene.h>

namespace BoulderLeaf::Graphics
{
	class blCylinderDemoScene : public blDemoScene
	{
	private:
		blMeshBaseResourcePtr mBoxMeshResource;
		blSceneResourcePtr mSceneResource;
		blShaderResourcePtr mShaderResource;
		blMaterialResourcePtr mMaterialResource;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		RenderMeshDataInstanced mDrawData;
		Math::Matrix4x4 mWorld = Matrix4x4::Identity();
	private:
		float mTheta;
		float mPhi;
		float mRadius = 5.0f;
		VirtualCamera mCamera;
	public:
		blCylinderDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
		virtual void Draw() override;
		virtual void Update(const Metrics::blTime& gameTime) override;
	};
}