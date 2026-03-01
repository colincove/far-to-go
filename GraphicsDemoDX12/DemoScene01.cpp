#include <DemoScene01.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blDemoScene01::blDemoScene01(std::shared_ptr<API> graphicsAPI)
		: mGraphicsAPI(graphicsAPI)
	{
		mBoxMeshResource = std::reinterpret_pointer_cast<blMeshBaseResource>(
			blResourceManager::Get().CreateResource<StandardMeshResource>(Cube()));

		mShaderResource = blResourceManager::Get().CreateResource<blShaderResource>(blShader(
			"box_example",
			std::vector<blShader::Parameter>
			{
				{1, 0, 0, blShader::Parameter::ConstantBuffer}
			},
			StandardVertexDefinition::Description));

		mMaterialResource = blResourceManager::Get().CreateResource<blMaterialResource>(mShaderResource, false);
		mSceneResource = blResourceManager::Get().CreateResource<blSceneResource>();

		// This does not work for some reason. Not sure why.
		/*mObjectConstantBufferResource = blResourceManager::Get().CreateResource<blStandardObjectConstantsBufferResource>(std::vector<blStandardObjectConstants>{
			{
				.WorldViewProj = Math::Matrix4x4::Identity()
			}
		});*/

		mObjectConstantBufferResource = blResourceManager::Get().CreateResource<blStandardObjectConstantsBufferResource>();
		mObjectConstantBufferResource->GetDataMutable().reserve(1000);

		mInstanceDataResource = std::make_shared<blStandardObjectConstantsBuffer>();
		mInstanceDataResource->reserve(1000);
		mInstanceDataResource->push_back({ Math::Matrix4x4::Identity()});

		mDrawData.instanceData = std::reinterpret_pointer_cast<blDataBufferInterface>(mInstanceDataResource);
		mDrawData.constantBuffer = std::reinterpret_pointer_cast<blDataBufferInterfaceResource>(mObjectConstantBufferResource);
		mDrawData.group = blRenderGroups::Default;
		mDrawData.material = mMaterialResource;
		mDrawData.mesh = mBoxMeshResource;
	}

	void blDemoScene01::Draw()
	{
		mRenderItems.clear();
		mRenderItems.push_back(mDrawData);
		mGraphicsAPI->DrawScene(mRenderItems, mSceneResource);
	}

	void blDemoScene01::Update()
	{
		// TODO update object data
	}
}