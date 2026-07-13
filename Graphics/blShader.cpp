#include <blShader.h>

BL_RESOURCE_IMPL(blShader);

namespace BoulderLeaf::Graphics
{
	blShaderResource_exp::blShaderResource_exp(
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
}