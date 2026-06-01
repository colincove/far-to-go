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

namespace BoulderLeaf::Graphics
{
	class blDemoScene01
	{
	private:
		blMeshBaseResourcePtr mBoxMeshResource;
		blSceneResourcePtr mSceneResource;
		blShaderResourcePtr mShaderResource;
		blMaterialResourcePtr mMaterialResource;
		std::shared_ptr<API> mGraphicsAPI;
		blStandardObjectConstantsBufferResourcePtr mObjectConstantBufferResource;
		blStandardObjectConstantsBufferPtr mInstanceDataResource;
		RenderMeshDataInstanced mDrawData;
		Math::Matrix4x4 mWorld = Matrix4x4::Identity();
	private:
		float mTheta;
		float mPhi;
		float mRadius = 5.0f;
		//XMFLOAT4X4 mWorld = XMFLOAT4X4_Identity();
		//XMFLOAT4X4 mView = XMFLOAT4X4_Identity();
		VirtualCamera mCamera;
	public:
		blDemoScene01(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
		void Draw();
		void Update();
	};
}