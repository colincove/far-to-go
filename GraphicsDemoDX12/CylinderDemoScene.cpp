#include <CylinderDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blCylinderDemoScene::blCylinderDemoScene(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window)
		: blDemoScene(graphicsAPI, window),
		mTheta(1.5f * PIf), 
		mPhi(PIfDIV4),
		mCamera(1, 1000, 0.25f * PIf, window->AspectRatio())
	{
		mBoxMeshResource = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(CreateCylinder(
				2,	//bottomRadius
				1,	//topRadius
				3,	//height
				20,	//sliceCount
				10	//stackCount
			)));

		mShaderResource = blResourceManager::Get().CreateResourceWithName<blShaderResource>( 
			L"graphics_dx12_demoscene01",
			blShader(
			"graphics_dx12_demoscene01",
			std::vector<blShader::Parameter>
			{
				{1, 0, 0, blShader::Parameter::ConstantBuffer}
			},
			StandardVertexDefinition::Description));

		mMaterialResource = blResourceManager::Get().CreateResource<blMaterialResource>(mShaderResource, false);
		mSceneResource = blResourceManager::Get().CreateResource<blSceneResource>();
		mObjectConstantBufferResource = blResourceManager::Get().CreateResourceWithName<blStandardObjectConstantsBufferResource>(
			L"blDemoScene01 Standard Object Constants Buffer");

		mObjectConstantBufferResource->GetDataMutable().reserve(1000);
		mObjectConstantBufferResource->GetDataMutable().push_back({Math::Matrix4x4::Identity()});
		mObjectConstantBufferResource->GetDataMutable().push_back({ Math::Matrix4x4::Identity() });

		mDrawData.constantBuffer = std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource);
		mDrawData.group = blRenderGroups::Default;
		mDrawData.material = mMaterialResource;
		mDrawData.mesh = mBoxMeshResource;
	}

	void blCylinderDemoScene::Draw()
	{
		mGraphicsAPI->DrawMeshInstanced(mDrawData, mSceneResource);
	}

	void blCylinderDemoScene::Update(const Metrics::blTime& gameTime)
	{
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
		
		const Matrix4x4 translate = Matrix4x4::TranslationMatrix(-2, 0, 3);
		const Matrix4x4 translate2 = Matrix4x4::TranslationMatrix(0, 0, 5);
		const Matrix4x4 view = Matrix4x4::ViewMatrix(pos, target, up) * translate;
		const Matrix4x4 view2 = Matrix4x4::ViewMatrix(pos, target, up) * translate2;
		const Matrix4x4 world = Matrix4x4::TranslationMatrix(Vector3());
		const Matrix4x4 proj = mCamera.GetProjectionMatrix();
		const Matrix4x4 worldViewProj = world * view * proj;
		const Matrix4x4 worldViewProj2 = world * view2 * proj;

		//TODO
		//we currently do not update constant buffer resources. So it does not change. 

		// We transpose the matrix here because the shader expects column-major matrices, but our math library uses row-major matrices. Transposing converts between these two conventions.
		mObjectConstantBufferResource->GetDataMutable()[0].WorldViewProj = worldViewProj.Transpose();
		mObjectConstantBufferResource->GetDataMutable()[1].WorldViewProj = worldViewProj2.Transpose();

		mGraphicsAPI->MarkResourceDirty(mObjectConstantBufferResource->GetId());
	}
}