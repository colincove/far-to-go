#include <blShapesDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blShapesDemoScene::blShapesDemoScene(
		API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blDemoScene(graphicsAPI, window, resourceContainer),
		mTheta(1.5f * PIf),
		mPhi(PIfDIV4),
		mCurrentShapeToDraw(0),
		mTimeSinceLastChange(),
		mCamera(1, 1000, 0.25f * PIf, window->AspectRatio())
	{
		mBoxMeshResource = CubeResource(resourceContainer);

		mCylinderMeshResource = CreateCylinderResource(resourceContainer,
			2,    //bottomRadius
			1,    //topRadius
			3,    //height
			20,   //sliceCount
			10    //stackCount
		);

		mGeosphereMeshResource = CreateGeosphereResource(resourceContainer,
			2,    //radius
			1    //numSubdivisions
		);

		mCompositeMeshResource = resourceContainer->CreateResourceOfTypeWithDynamicSize<blCompositeMeshResource>(
			L"ShapesDemoCompositeMesh",
			std::vector<blResourceRefOfType<blIndexedMeshResource>> 
		{
			mBoxMeshResource.GetRef(),
			mCylinderMeshResource.GetRef(),
			mGeosphereMeshResource.GetRef()
		});

		mMaterialResource = CreateDefaultMaterial(resourceContainer);
		mSceneResource = resourceContainer->CreateResourceOfType<blSceneResource>(L"Scene");

		blResourceHandleOfType<blBufferDescriptionResource> constantBufferDesc = BufferDescriptionResource(
			resourceContainer, { BufferFormat::BoulderLeaf, blStandardObjectConstantsDefinition::Description, 256 });

		mObjectConstantBufferResource = CreateArrayBufferResource(
			resourceContainer,
			L"ObjectConstantBuffer",
			constantBufferDesc.GetRef(),
			sizeof(blStandardObjectConstants),
			2
		);

		mObjectConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType(mObjectConstantBufferResource->mBufferResourceRef);

		mRenderData = std::vector<RenderCompositeMeshDataInstanced>
		{
			RenderCompositeMeshDataInstanced
			{
				blRenderGroups::Default,
				mObjectConstantBufferResource,
				mCompositeMeshResource,
				mMaterialResource,
				mBoxMeshResource.GetId(), //submeshId
				0
			},
			RenderCompositeMeshDataInstanced
			{
				blRenderGroups::Default,
				mObjectConstantBufferResource,
				mCompositeMeshResource,
				mMaterialResource,
				mCylinderMeshResource.GetId(), //submeshId
				1
			},
			RenderCompositeMeshDataInstanced
			{
				blRenderGroups::Default,
				mObjectConstantBufferResource,
				mCompositeMeshResource,
				mMaterialResource,
				mGeosphereMeshResource.GetId(), //submeshId
				2
			}
		};
	}

	void blShapesDemoScene::Draw()
	{
		mGraphicsAPI->DrawCompositeMeshInstanced(mRenderData[mCurrentShapeToDraw]);
	}

	void blShapesDemoScene::Update(const Metrics::blTime& gameTime)
	{
		if (gameTime.Current() - mTimeSinceLastChange > Metrics::blTime::Seconds(3))
		{
			mCurrentShapeToDraw = mCurrentShapeToDraw >= mRenderData.size() - 1 ? 0 : mCurrentShapeToDraw + 1;
			mTimeSinceLastChange = gameTime.Current();
		}

		using namespace Math;
		static float rotationSpeed = 0.5f; // radians per second
		float deltaSeconds = gameTime.DeltaSeconds();
		mTheta += gameTime.DeltaSeconds() * rotationSpeed;
		mPhi += gameTime.DeltaSeconds() * rotationSpeed;
		// Convert Spherical to Cartesian coordinates.
		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);

		Vector4 pos = Vector4(x, y, z, 1.0f);


		//Vector4 pos = Vector4::Point3D(5, 2, 2);
		Vector4 target = Vector4::Zero();
		Vector4 up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		const Matrix4x4 translate = Matrix4x4::TranslationMatrix(0, 0, 3);
		const Matrix4x4 view = Matrix4x4::ViewMatrix(pos, target, up) * translate;
		const Matrix4x4 world = Matrix4x4::TranslationMatrix(Vector3());
		const Matrix4x4 proj = mCamera.GetProjectionMatrix();
		const Matrix4x4 worldViewProj = world * view * proj;

		// We transpose the matrix here because the shader expects column-major matrices, but our math library uses row-major matrices. Transposing converts between these two conventions.
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(0).WorldViewProj = worldViewProj;
		mObjectConstantBufferListResource.MarkDirty();
	}
}