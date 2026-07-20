#include <blTerrainDemo.h>
#include <blMeshLibrary.h>
#include <blShader.h>
#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	blTerrainDemoScene::blTerrainDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blDemoScene(graphicsAPI, window, resourceContainer), mCamera(1, 1000, 0.25f * PIf, window->AspectRatio())
	{
		static const uint32_t size = 2;
		SetMeshAndShaderData(CreateTerrainResource(resourceContainer, size));
		
		for (blTerrainVertex& vert : blListResource::Iterator<blTerrainVertex>(mTerrainVertexBuffer.GetMutable()))
		{
			vert.Height = ((float) rand() / (float) RAND_MAX);
		}

		blResourceHandleOfType<blBufferDescriptionResource> constantBufferDesc = BufferDescriptionResource(
			resourceContainer, { BufferFormat::BoulderLeaf, blTerrainObjectConstantsElementDescriptions, 256 });

		mObjectConstantBufferResource = CreateArrayBufferResource(
			resourceContainer,
			L"ObjectConstantBuffer",
			constantBufferDesc.GetRef(),
			sizeof(blTerrainObjectConstants),
			1
		);

		mObjectConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType<blListResource>(mObjectConstantBufferResource->mBufferResourceRef);
		mObjectConstantBufferListResource->GetMutable<blTerrainObjectConstants>(0) = { Matrix4x4::TranslationMatrix(-2, 0, 3), 1.0f , size, 0, 0 };
		blTerrainObjectConstants& constantsRef = mObjectConstantBufferListResource->GetMutable<blTerrainObjectConstants>(0);
		blTerrainVertex& terrainVertexRef = mTerrainVertexBuffer->GetMutable<blTerrainVertex>(0);

		CalculateHeightBoundsForTerrain(&terrainVertexRef, 
			mTerrainVertexBuffer->mCount,
			constantsRef.MinHeight,
			constantsRef.MaxHeight);

		blResourceHandleOfType<blBufferDescriptionResource> passConstantBufferDesc = BufferDescriptionResource(
			resourceContainer, { BufferFormat::BoulderLeaf, cbPassDescription, 256 });

		mPassConstantBufferResource = CreateArrayBufferResource(
			resourceContainer,
			L"PassConstantBuffer",
			passConstantBufferDesc.GetRef(),
			sizeof(cbPass),
			1
		);

		mPassConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType<blListResource>(mPassConstantBufferResource->mBufferResourceRef);

		mDrawData.constantBuffers = resourceContainer->CreateResourceOfTypeWithDynamicSize<blConstantBufferResource>(
			L"blTerrainDemoScene Constant Buffer",
			mObjectConstantBufferResource.GetRef(),
			std::vector<blResourceRefOfType<blArrayBufferResource>> { mPassConstantBufferResource.GetRef() }
		);
	}

	void blTerrainDemoScene::SetMeshAndShaderData(blResourceHandleOfType<blIndexedMeshResource> mesh)
	{
		blResourceContainer* container = mesh.GetContainer();

		mTerrainMesh = mesh;
		mDrawData.mesh = mTerrainMesh;
		mTerrainVertexArrayResource = container->CreateHandleFromRefOfType<blArrayBufferResource>(mTerrainMesh->mArrayBufferResourceRef);
		mTerrainVertexBuffer = container->CreateHandleFromRefOfType<blListResource>(mTerrainVertexArrayResource->mBufferResourceRef);

		const blArrayBufferResource& vertexArrayBuffer = *container->CreateHandleFromRefOfType<blArrayBufferResource>(mesh->mArrayBufferResourceRef);

		// prepare shader parameters
		std::vector<blShader::Parameter> params =
		{
			{1, 0, 0, blShader::Parameter::ConstantBuffer },
			{ 1, 1, 0, blShader::Parameter::ConstantBuffer }
		};

		const std::string shaderName = "graphics_dx12_terrain_demo";

		// create shader resource
		blResourceHandleOfType<blShaderResource> handle =
			container->CreateResourceOfTypeWithDynamicSize<blShaderResource>(
				L"ShaderResource",
				shaderName,
				params.data(),
				static_cast<uint32_t>(params.size()),
				vertexArrayBuffer.mDescriptionResourceRef
			);

		mMaterialResource = container->CreateResourceOfType<blMaterialResource>(
			L"MaterialResource",
			handle.GetRef(),
			true /*instanced*/
		);

		mDrawData.material = mMaterialResource;
	}

	void blTerrainDemoScene::Draw()
	{
		mGraphicsAPI->DrawMeshInstancedV2(mDrawData);
	}
	void blTerrainDemoScene::Update(const Metrics::blTime& gameTime)
	{
		mPassConstantBufferListResource->GetMutable<cbPass>(0) = CalculatePassData(mCamera, gameTime);
		mPassConstantBufferListResource.MarkDirty();
		mPassConstantBufferResource.MarkDirty();
	}
}