#include <ShapesWithPassDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	ShapesWithPassDemoScene::ShapesWithPassDemoScene(
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

		mMaterialResource = CreateDefaultMaterialWithPassConstants(resourceContainer);
		mSceneResource = resourceContainer->CreateResourceOfType<blSceneResource>(L"Scene");

		blResourceHandleOfType<blBufferDescriptionResource> constantBufferDesc = BufferDescriptionResource(
			resourceContainer, { BufferFormat::BoulderLeaf, blStandardObjectConstantsDefinition::Description, 256 });

		mObjectConstantBufferResource = CreateArrayBufferResource(
			resourceContainer,
			L"ObjectConstantBuffer",
			constantBufferDesc.GetRef(),
			sizeof(blStandardObjectConstants),
			10
		);

		mObjectConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType(mObjectConstantBufferResource->mBufferResourceRef);
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(0) = { Math::Matrix4x4::Identity() };
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(1) = { Math::Matrix4x4::TranslationMatrix(1, 2, 3)};
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(2) = { Math::Matrix4x4::TranslationMatrix(2, 2, 2) };
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(3) = { Math::Matrix4x4::TranslationMatrix(-2, -1, 0) };
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(4) = { Math::Matrix4x4::TranslationMatrix(4, 4, 4) };
		mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(5) = { Math::Matrix4x4::TranslationMatrix(1, 1, 1) };

		blResourceHandleOfType<blBufferDescriptionResource> passConstantBufferDesc = BufferDescriptionResource(
			resourceContainer, { BufferFormat::BoulderLeaf, cbPassDescription, 256 });

		mPassConstantBufferResource = CreateArrayBufferResource(
			resourceContainer,
			L"PassConstantBuffer",
			passConstantBufferDesc.GetRef(),
			sizeof(cbPass),
			1
		);

		mPassConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType(mPassConstantBufferResource->mBufferResourceRef);

		mConstantBufferResource = resourceContainer->CreateResourceOfTypeWithDynamicSize<blConstantBufferResource>(
			L"ShapesWithPassDemoScene Constant Buffer",
			mObjectConstantBufferResource.GetRef(),
			std::vector<blResourceRefOfType<blArrayBufferResource>> { mPassConstantBufferResource.GetRef() }
		);

		mRenderData = std::vector<RenderCompositeMeshDataWithPassConstants>
		{
			RenderCompositeMeshDataWithPassConstants
			{
				blRenderGroups::Default,
				mObjectConstantBufferResource,
				mPassConstantBufferResource,
				mCompositeMeshResource,
				mMaterialResource,
				std::vector<BoulderLeaf::blResourceId> {mBoxMeshResource.GetId()},
				0,
				mConstantBufferResource
			},
			RenderCompositeMeshDataWithPassConstants
			{
				blRenderGroups::Default,
				mObjectConstantBufferResource,
				mPassConstantBufferResource,
				mCompositeMeshResource,
				mMaterialResource,
				std::vector<BoulderLeaf::blResourceId> {mGeosphereMeshResource.GetId()},
				1,
				mConstantBufferResource
			},
			RenderCompositeMeshDataWithPassConstants
			{
				blRenderGroups::Default,
				mObjectConstantBufferResource,
				mPassConstantBufferResource,
				mCompositeMeshResource,
				mMaterialResource,
				std::vector<BoulderLeaf::blResourceId> 
				{
					mGeosphereMeshResource.GetId(),
					mGeosphereMeshResource.GetId(),
					mGeosphereMeshResource.GetId(),
					mCylinderMeshResource.GetId(),
				},
				2,
			mConstantBufferResource
			}
		};
	}

	void ShapesWithPassDemoScene::Draw()
	{
		mGraphicsAPI->DrawCompositeMeshWithPass(mRenderData[mCurrentShapeToDraw]);
	}

	void ShapesWithPassDemoScene::Update(const Metrics::blTime& gameTime)
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

		const Matrix4x4 translate = Matrix4x4::TranslationMatrix(0, 0, 0);
		const Matrix4x4 view = Matrix4x4::ViewMatrix(pos, target, up)/* * translate*/;
		const Matrix4x4 world = Matrix4x4::TranslationMatrix(Vector3());
		const Matrix4x4 proj = mCamera.GetProjectionMatrix();
		const Matrix4x4 worldViewProj = world * view * proj;

		for (int i = 0; i < mObjectConstantBufferListResource->mCount; ++i)
		{
			mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(i).WorldViewProj = worldViewProj;
		}

		mObjectConstantBufferResource.MarkDirty();
		mObjectConstantBufferListResource.MarkDirty();
		
		// We transpose the matrix here because the shader expects column-major matrices, but our math library uses row-major matrices. Transposing converts between these two conventions.
		cbPass& passData = mPassConstantBufferListResource->GetMutable<cbPass>(0);
		passData.view = worldViewProj;
		mPassConstantBufferListResource.MarkDirty();
		mPassConstantBufferResource.MarkDirty();
	}
}