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
#include <blCompositeMesh.h>
#include <DemoScene.h>

namespace BoulderLeaf::Graphics
{
	class ShapesWithPassDemoScene : public blDemoScene
	{
	private:
		blMeshBaseResourcePtr mBoxMeshResource;
		blMeshBaseResourcePtr mCylinderMeshResource;
		blMeshBaseResourcePtr mGeosphereMeshResource;
		blCompositeMeshResourcePtr mCompositeMeshResource;

		std::vector<RenderCompositeMeshDataWithPassConstants> mRenderData;

		blSceneResourcePtr mSceneResource;
		blShaderResourcePtr mShaderResource;
		blMaterialResourcePtr mMaterialResource;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		blVertexBufferWithPassBufferResourcePtr mConstantBufferResource;
		blDataElementBufferResourcePtr mConstantBufferPassResource;
		
		Math::Matrix4x4 mProjMatrix = Matrix4x4::Identity();
		Math::Matrix4x4 mWorld = Matrix4x4::Identity();
	private:
		float mTheta;
		float mPhi;
		float mRadius = 5.0f;
		VirtualCamera mCamera;
		Metrics::blTime::time mTimeSinceLastChange;
		int mCurrentShapeToDraw;
	public:
		ShapesWithPassDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
		void Draw();
		void Update(const Metrics::blTime& gameTime);
	};
}