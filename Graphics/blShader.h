#pragma once
#include <string>
#include <vector>
#include <Resources/blResource.h>
#include <Resources/blResourcesExprimental.h>
#include <blBuffer.h>

namespace BoulderLeaf::Graphics
{
	class blShader
	{
	public:
		struct Parameter
		{
			int Count;
			int Register;
			int Space;

			enum Type
			{
				ConstantBuffer,
				ShaderResource,
				UnorderedAccess,
				Sampler
			} type;
		};

		blShader() {}
		~blShader() {}

		std::string mName;
		std::vector<Parameter> mParameters;
		std::vector<BufferElementDescription> mLayout;

		blShader(const std::string& name, const std::vector<Parameter>&& parameters,
			const std::vector<BufferElementDescription>& layout)
			: mName(name), mParameters(parameters), mLayout(layout) 
		{
		}
	};

	struct blShaderResource_exp : blBaseResource
	{
		blStringResource mNameStringResource;
		blListResource mParameters;
		blResourceRefOfType<blBufferDescriptionResource> mDescriptionResourceRef;

		blShaderResource_exp(
			blResourceStream& stream,
			const std::string& name, 
			const blShader::Parameter* parameters,
			uint32_t numberOfParameters,
			blResourceRefOfType<blBufferDescriptionResource> layout);
	};
}

BL_RESOURCE(blShader, eResourceType::Shader)