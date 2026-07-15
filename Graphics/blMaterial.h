#pragma once

#include <blShader.h>
#include <Resources/blResource.h>
#include <Resources/blResourcesExprimental.h>

namespace BoulderLeaf::Graphics
{
	class blMaterial
	{
	public:
		blMaterial(blShaderResourcePtr shader, bool instanced);
		blShaderResourcePtr shader;
	private:
		bool mInstanced;
	public:
		bool IsInstanced() const { return mInstanced; }
	};

	struct blMaterialResource_exp : public blBaseResource
	{
		bool mInstanced;
		blResourceRefOfType<blShaderResource_exp> mShaderResourceRef;

		blMaterialResource_exp(
			blResourceRefOfType<blShaderResource_exp> shaderResourceRef,
			bool instanced);
	};
}

BL_RESOURCE(blMaterial, eResourceType::Material);