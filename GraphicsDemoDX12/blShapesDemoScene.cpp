#include <blShapesDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blShapesDemoScene::blShapesDemoScene(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window, blResourceContainer* resourceContainer)
		: blDemoScene(graphicsAPI, window, resourceContainer),
		mTheta(1.5f * PIf),
		mPhi(PIfDIV4),
		mCurrentShapeToDraw(0),
		mTimeSinceLastChange(),
		mCamera(1, 1000, 0.25f * PIf, window->AspectRatio())
	{
		mBoxMeshResource = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(Cube()));

		mCylinderMeshResource = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(CreateCylinder(
				2,	//bottomRadius
				1,	//topRadius
				3,	//height
				20,	//sliceCount
				10	//stackCount
			)));

		mGeosphereMeshResource = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(CreateGeosphere(
				1.5f,	//radius
				4	//numSubdivisions
			)));

		mCompositeMeshResource = blResourceManager::Get().CreateResource<blCompositeMeshResource>(
			3,
			mBoxMeshResource,
			mCylinderMeshResource,
			mGeosphereMeshResource
		);

		mShaderResource = blResourceManager::Get().CreateResource<blShaderResource>(blShader(
			"graphics_dx12_demoscene01",
			std::vector<blShader::Parameter>
		{
			{1, 0, 0, blShader::Parameter::ConstantBuffer}
		},
			StandardVertexDefinition::Description));

		mMaterialResource = blResourceManager::Get().CreateResource<blMaterialResource>(mShaderResource, false);
		mSceneResource = blResourceManager::Get().CreateResource<blSceneResource>();

		mObjectConstantBufferResource = blResourceManager::Get().CreateResource<blStandardObjectConstantsBufferResource>();
		mObjectConstantBufferResource->GetDataMutable().reserve(1000);
		mObjectConstantBufferResource->GetDataMutable().push_back({ Math::Matrix4x4::Identity() });

		mRenderData = std::vector<RenderCompositeMeshDataInstanced>
		{
			RenderCompositeMeshDataInstanced
			{
				blRenderGroups::Default,
				std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource),
				mCompositeMeshResource,
				mMaterialResource,
				mBoxMeshResource->GetId(), //submeshId
				0
			},
			RenderCompositeMeshDataInstanced
			{
				blRenderGroups::Default,
				std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource),
				mCompositeMeshResource,
				mMaterialResource,
				mCylinderMeshResource->GetId(), //submeshId
				1
			},
			RenderCompositeMeshDataInstanced
			{
				blRenderGroups::Default,
				std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource),
				mCompositeMeshResource,
				mMaterialResource,
				mGeosphereMeshResource->GetId(), //submeshId
				2
			}
		};
	}

	void blShapesDemoScene::Draw()
	{
		mGraphicsAPI->DrawCompositeMeshInstanced(mRenderData[mCurrentShapeToDraw], mSceneResource);
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
		mObjectConstantBufferResource->GetDataMutable()[0].WorldViewProj = worldViewProj.Transpose();
		mGraphicsAPI->MarkResourceDirty(mObjectConstantBufferResource->GetId());
	}
}