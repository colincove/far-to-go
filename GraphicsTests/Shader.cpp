#include <gtest/gtest.h>
#include <blShader.h>
#include <Resources/blResources.h>
#include <blBuffer.h>

using namespace BoulderLeaf;

TEST(Shader, ResourceContents)
{
	using namespace Graphics;

	blResourceContainerPool pool;
	blResourceContainer* container = pool.GetContainer(
		1000000 * 256, // 256mb
		10000
	);

	// create a simple buffer description to use as layout ref
	Graphics::BufferDescription desc =
	{
		Graphics::BufferFormat::DX12,
		{ {"A", Graphics::BufferElementType::Float}, {"B", Graphics::BufferElementType::Float2} }
	};

	blResourceHandleOfType<blBufferDescriptionResource> descResource =
		container->CreateResourceOfTypeWithDynamicSize<blBufferDescriptionResource>(L"BufferDescriptionResource", desc);

	// prepare shader parameters
	std::vector<blShader::Parameter> params =
	{
		{ 1, 0, 0, blShader::Parameter::ConstantBuffer },
		{ 2, 1, 0, blShader::Parameter::ShaderResource }
	};

	const std::string shaderName = "TestShader";

	// create shader resource
	blResourceHandleOfType<blShaderResource> handle =
		container->CreateResourceOfTypeWithDynamicSize<blShaderResource>(
			L"ShaderResource",
			shaderName,
			params.data(),
			static_cast<uint32_t>(params.size()),
			descResource.GetRef()
		);

	ASSERT_TRUE(handle.IsValid());

	const blShaderResource& resource = *handle;

	// verify name
	EXPECT_EQ(std::string(resource.mNameStringResource.GetStringView()), shaderName);

	// verify layout ref
	EXPECT_EQ(resource.mDescriptionResourceRef.GetId(), descResource.GetId());

	// verify parameters
	ASSERT_EQ(resource.mParameters.mCount, params.size());

	for (uint32_t i = 0; i < resource.mParameters.mCount; ++i)
	{
		const blShader::Parameter& p = resource.mParameters.Get<blShader::Parameter>(i);
		EXPECT_EQ(p.Count, params[i].Count);
		EXPECT_EQ(p.Register, params[i].Register);
		EXPECT_EQ(p.Space, params[i].Space);
		EXPECT_EQ(p.type, params[i].type);
	}
}
