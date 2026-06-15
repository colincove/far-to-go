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

namespace BoulderLeaf::Graphics
{
	class blDemoScene02
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
		std::shared_ptr<API> mGraphicsAPI;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		Math::Matrix4x4 mWorld = Matrix4x4::Identity();
	private:
		float mTheta;
		float mPhi;
		float mRadius = 5.0f;
		VirtualCamera mCamera;
	public:
		blDemoScene02(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
		void Draw();
		void Update(Metrics::blTime& gameTime);
	};
}