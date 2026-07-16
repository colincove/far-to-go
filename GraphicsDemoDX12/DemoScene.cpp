#include <DemoScene.h>
#include <Resources/blResourceHandleOfType.h>
#include <blMaterial.h>
#include <blBuffer.h>
#include <blMesh.h>
#include <blMeshLibrary.h>

namespace BoulderLeaf::Graphics
{
	blDemoScene::blDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer)
		: mGraphicsAPI(graphicsAPI), mWindow(window), mResourceContainer(resourceContainer)
	{

	}

	blResourceHandleOfType<blMaterialResource> CreateDefaultMaterial(blResourceContainer* container)
	{
		// prepare shader parameters
		std::vector<blShader::Parameter> params =
		{
			{1, 0, 0, blShader::Parameter::ConstantBuffer}
		};

		const std::string shaderName = "graphics_dx12_demoscene01";

		blResourceHandleOfType<blBufferDescriptionResource> desc = BufferDescriptionResource(
			container, { BufferFormat::BoulderLeaf, StandardVertexDefinition::Description });

		// create shader resource
		blResourceHandleOfType<blShaderResource> handle =
			container->CreateResourceOfTypeWithDynamicSize<blShaderResource>(
				L"ShaderResource",
				shaderName,
				params.data(),
				static_cast<uint32_t>(params.size()),
				desc.GetRef()
			);

		blResourceHandleOfType<blMaterialResource> materialResource = container->CreateResourceOfType<blMaterialResource>(
			L"MaterialResource",
			handle.GetRef(),
			true /*instanced*/
		);

		return materialResource;
	}

	blResourceHandleOfType<blMaterialResource> CreateDefaultMaterialWithPassConstants(blResourceContainer* container)
	{
		// prepare shader parameters
		std::vector<blShader::Parameter> params =
		{
			{1, 0, 0, blShader::Parameter::ConstantBuffer },
			{ 1, 1, 0, blShader::Parameter::ConstantBuffer }
		};

		const std::string shaderName = "graphics_dx12_shapes_demo";

		blResourceHandleOfType<blBufferDescriptionResource> desc = BufferDescriptionResource(
			container, { BufferFormat::BoulderLeaf, StandardVertexDefinition::Description });

		// create shader resource
		blResourceHandleOfType<blShaderResource> handle =
			container->CreateResourceOfTypeWithDynamicSize<blShaderResource>(
				L"ShaderResource",
				shaderName,
				params.data(),
				static_cast<uint32_t>(params.size()),
				desc.GetRef()
			);

		blResourceHandleOfType<blMaterialResource> materialResource = container->CreateResourceOfType<blMaterialResource>(
			L"MaterialResource",
			handle.GetRef(),
			true /*instanced*/
		);

		return materialResource;
	}
}