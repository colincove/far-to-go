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
	class blShapesDemoScene : public blDemoScene
	{
	private:
		blMeshBaseResourcePtr mBoxMeshResource;
		blMeshBaseResourcePtr mCylinderMeshResource;
		blMeshBaseResourcePtr mGeosphereMeshResource;
		blCompositeMeshResourcePtr mCompositeMeshResource;

		std::vector<RenderCompositeMeshDataInstanced> mRenderData;

		blSceneResourcePtr mSceneResource;
		blShaderResourcePtr mShaderResource;
		blMaterialResourcePtr mMaterialResource;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		Math::Matrix4x4 mWorld = Matrix4x4::Identity();
	private:
		float mTheta;
		float mPhi;
		float mRadius = 5.0f;
		VirtualCamera mCamera;
		Metrics::blTime::time mTimeSinceLastChange;
		int mCurrentShapeToDraw;
	public:
		blShapesDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
		void Draw();
		void Update(const Metrics::blTime& gameTime);
	};
}