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
	struct cbPass
	{
		BoulderLeaf::Math::Matrix4x4 view;
		BoulderLeaf::Math::Matrix4x4 invView;
		BoulderLeaf::Math::Matrix4x4 Proj;
		BoulderLeaf::Math::Matrix4x4 invProj;
		BoulderLeaf::Math::Matrix4x4 viewProj;
		BoulderLeaf::Math::Matrix4x4 invViewProj;
		BoulderLeaf::Math::Vector3 eyePosW;
		float cbPerObjectPad1;
		BoulderLeaf::Math::Vector2 renderTargetSize;
		BoulderLeaf::Math::Vector2 invRenderTargetSize;
		float nearZ;
		float farZ;
		float totalTime;
		float deltaTime;
	};

	static const std::vector<BufferElementDescription> cbPassDescription =
	{
		{
				"View",
				BufferElementType::Matrix4x4
		},
		{
				"InvView",
				BufferElementType::Matrix4x4
		},
		{
				"Projection",
				BufferElementType::Matrix4x4
		},
		{
				"invProj",
				BufferElementType::Matrix4x4
		},
		{
				"viewProj",
				BufferElementType::Matrix4x4
		},
		{
				"invViewProj",
				BufferElementType::Matrix4x4
		},
		{
				"eyePosW",
				BufferElementType::Float3
		},
		{
				"cbPerObjectPad1",
				BufferElementType::Float
		},
		{
				"renderTargetSize",
				BufferElementType::Float2
		},
		{
				"invRenderTargetSize",
				BufferElementType::Float2
		},
		{
				"nearZ",
				BufferElementType::Float
		},
		{
				"farZ",
				BufferElementType::Float
		},
		{
				"totalTime",
				BufferElementType::Float
		},
		{
				"deltaTime",
				BufferElementType::Float
		},
	};

	class ShapesWithPassDemoScene : public blDemoScene
	{
	private:
		bool mDidSetupPassData;
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
		
		cbPass mPassData;
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
		ShapesWithPassDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
		void Draw();
		void Update(const Metrics::blTime& gameTime);
	};
}