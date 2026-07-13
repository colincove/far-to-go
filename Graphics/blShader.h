#pragma once
#include <string>
#include <vector>
#include <Resources/blResource.h>
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

	class blInlineShader
	{
	public:
		static uint64_t CalculateSize(blShader& shader);
	};
}

BL_RESOURCE(blShader, eResourceType::Shader)