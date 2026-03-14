#include <DemoScene01.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blDemoScene01::blDemoScene01(
		std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window)
		: mGraphicsAPI(graphicsAPI), 
		mTheta(1.5f * PIf), 
		mPhi(PIfDIV4),
		mCamera(1, 1000, 0.25f * PIf, window->AspectRatio())
	{
		mBoxMeshResource = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(Cube()));

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
		mObjectConstantBufferResource->GetDataMutable().push_back({Math::Matrix4x4::Identity()});

		// this reinterpret_pointer_cast is not working right. Review.
		// It is likely due to how resources are designed. It is bad. THe "data" might be compatible,
		// but the memory layout and class structure of the reousrces themselves are completely different. 
		// another issue likely due to how resources actually own data. yuck. 
		//mDrawData.constantBuffer = std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource);

		mDrawData.constantBuffer = mObjectConstantBufferResource;
		mDrawData.group = blRenderGroups::Default;
		mDrawData.material = mMaterialResource;
		mDrawData.mesh = mBoxMeshResource;
	}

	void blDemoScene01::Draw()
	{
		mGraphicsAPI->DrawMeshInstanced(mDrawData, mSceneResource);
	}

	void blDemoScene01::Update()
	{
		using namespace Math;
		mTheta += 0.05f;
		mPhi += 0.05f;
		// Convert Spherical to Cartesian coordinates.
		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);

		Vector4 pos = Vector4(x, y, z, 1.0f);
		Vector4 target = Vector4(0, 0, 0, 0);
		Vector4 up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
		
		//TODO
		//XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
		Matrix4x4 view = Matrix4x4::LookAt(pos, target, up);

		Matrix4x4 world = Matrix4x4::Identity();
		Matrix4x4 proj = mCamera.GetProjectionMatrix();
		Matrix4x4 worldViewProj = world * view * proj;
		mObjectConstantBufferResource->GetDataMutable()[0].WorldViewProj = worldViewProj.Transpose();
	}
}