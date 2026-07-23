#include <blMaterialAndLightingDemoScene.h>
#include <blMeshLibrary.h>
#include <blBufferTypes.h>
#include <blShader.h>
#include <blWindow.h>
#include <blDX12.h>
#include <imgui.h>
#include <algorithm>

namespace BoulderLeaf::Graphics
{
	blMaterialAndLightingDemoScene::blMaterialAndLightingDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: blDemoScene(graphicsAPI, window, resourceContainer), mCamera(1, 1000, 0.25f * PIf, window->AspectRatio()),
		mMaterialDropdown(std::vector<std::string>{"Material 0", "Material 1", "Material 2"}, "MaterialSelector", "Material")
	{
		// create geosphere mesh resource
		mGeosphereMesh = CreateGeosphereResource(resourceContainer,
			2, // radius
			1  // numSubdivisions
		);

		mSceneResource = resourceContainer->CreateResourceOfType<blSceneResource>(L"Scene");

		// object constant buffer (per-object)
		blResourceHandleOfType<blBufferDescriptionResource> constantBufferDesc = BufferDescriptionResource(
			resourceContainer, { BufferFormat::BoulderLeaf, blStandardObjectConstantsDefinition::Description, 256 });

		mObjectConstantBufferResource = CreateArrayBufferResource(
			resourceContainer,
			L"ObjectConstantBuffer",
			constantBufferDesc.GetRef(),
			sizeof(blStandardObjectConstants),
			1
		);

		mObjectConstantBufferListResource = resourceContainer->CreateHandleFromRefOfType(mObjectConstantBufferResource->mBufferResourceRef);

		// pass constant buffer
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

		// create shader/material and hook mesh
		SetMeshAndShaderData(mGeosphereMesh);

		// combine object and pass buffers into constant buffer resource used by draw
		mDrawData.constantBuffers = resourceContainer->CreateResourceOfTypeWithDynamicSize<blConstantBufferResource>(
			L"MaterialAndLighting Constant Buffer",
			mObjectConstantBufferResource.GetRef(),
			std::vector<blResourceRefOfType<blArrayBufferResource>> { mPassConstantBufferResource.GetRef() }
		);

		// initialize dropdown position/size based on viewport
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		mMaterialDropdown.SetPosition(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + 30));
		mMaterialDropdown.SetSize(ImVec2((std::min)(300.0f, viewport->WorkSize.x), 60.0f));
	}

	void blMaterialAndLightingDemoScene::SetMeshAndShaderData(blResourceHandleOfType<blIndexedMeshResource> mesh)
	{
		blResourceContainer* container = mesh.GetContainer();

		mGeosphereMesh = mesh;
		mDrawData.mesh = mGeosphereMesh;

		const blArrayBufferResource& vertexArrayBuffer = *container->CreateHandleFromRefOfType<blArrayBufferResource>(mesh->mArrayBufferResourceRef);

		// prepare shader parameters: object CB and pass CB
		std::vector<blShader::Parameter> params =
		{
			{1, 0, 0, blShader::Parameter::ConstantBuffer},
			{1, 1, 0, blShader::Parameter::ConstantBuffer}
		};

		// reuse existing shader name from other demos
		const std::string shaderName = "graphics_dx12_terrain_demo";

		blResourceHandleOfType<blShaderResource> handle =
			container->CreateResourceOfTypeWithDynamicSize<blShaderResource>(
				L"ShaderResource",
				shaderName,
				params.data(),
				static_cast<uint32_t>(params.size()),
				vertexArrayBuffer.mDescriptionResourceRef
			);

		// create three identical material resources for selection
		mMaterials.clear();
		for (int i = 0; i < 3; ++i)
		{
			std::wstring name = L"MaterialResource";
			name += std::to_wstring(i);
			blResourceHandleOfType<blMaterialResource> mat = container->CreateResourceOfType<blMaterialResource>(
				name.c_str(),
				handle.GetRef(),
				true /*instanced*/
			);
			mMaterials.push_back(mat);
		}

		// default to first material
		if (!mMaterials.empty())
		{
			mMaterialResource = mMaterials[0];
			mDrawData.material = mMaterialResource;
		}
		mDrawData.group = blRenderGroups::Default;
	}

	void blMaterialAndLightingDemoScene::Draw()
	{
		// Use shared dropdown helper (member) to render material selector
		mMaterialDropdown.DrawWindow();

		int selected = mMaterialDropdown.GetSelected();

		if (selected >= 0 && selected < static_cast<int>(mMaterials.size()))
		{
			mDrawData.material = mMaterials[selected];
		}

		//mGraphicsAPI->DrawMeshInstancedV2(mDrawData);
	}

	void blMaterialAndLightingDemoScene::Update(const Metrics::blTime& gameTime)
	{
		// simple camera setup similar to terrain/other demos
		mCamera.mTranslation = Math::Vector4::Point3D(0.660871327, -14.8207569, 13.4129944);
		mCamera.LookAt(Math::Vector4::Point3D(0, 0, 0));

		cbPass& passData = mPassConstantBufferListResource->GetMutable<cbPass>(0);
		passData = CalculatePassData(mCamera, gameTime);
		mPassConstantBufferListResource.MarkDirty();
		mPassConstantBufferResource.MarkDirty();

		/*blStandardObjectConstants& objectConstants = mObjectConstantBufferListResource->GetMutable<blStandardObjectConstants>(0);
		objectConstants.WorldViewProj = objectConstants.World * passData.viewProj;
		objectConstants.WorldViewProjInverseTranspose = InverseTransposeForNormals(objectConstants.WorldViewProj);
		mObjectConstantBufferListResource.MarkDirty();
		mObjectConstantBufferResource.MarkDirty();*/
	}
}

