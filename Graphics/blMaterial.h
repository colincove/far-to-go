#pragma once

#include <blShader.h>
#include <Resources/blResources.h>

namespace BoulderLeaf::Graphics
{
	struct blMaterialResource : public blBaseResource
	{
		bool mInstanced;
		blResourceRefOfType<blShaderResource> mShaderResourceRef;

		blMaterialResource(
			blResourceRefOfType<blShaderResource> shaderResourceRef,
			bool instanced);
	};
}