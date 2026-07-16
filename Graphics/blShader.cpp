#include <blShader.h>
#include <Resources/blResourceHandleOfType.h>

namespace BoulderLeaf::Graphics
{
	blShaderResource::blShaderResource(
		blResourceStream& stream,
		const std::string& name,
		const blShader::Parameter* parameters,
		uint32_t numberOfParameters,
		blResourceRefOfType<blBufferDescriptionResource> layout)
		: blBaseResource(stream),
		mDescriptionResourceRef(layout),
		mNameStringResource(stream, name),
		mParameters(stream, numberOfParameters, parameters)
	{
	}

	blShader ShaderResourceToShader(const blResourceHandleOfType<blShaderResource> shaderResource)
	{
		const blResourceHandleOfType<blBufferDescriptionResource> desc = shaderResource.GetContainer()->CreateHandleFromRefOfType(
			shaderResource->mDescriptionResourceRef);

		std::vector<blShader::Parameter> params;

		for (const byte& paramData : blListResource::ConstIterator(&shaderResource->mParameters))
		{
			params.push_back(reinterpret_cast<const blShader::Parameter&>(paramData));
		}

		return blShader(
			std::string(shaderResource->mNameStringResource.GetStringView()),
			std::move(params),
			desc->GetBufferDescription().elementDescriptions
		);
	}
}