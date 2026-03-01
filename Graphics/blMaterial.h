#pragma once

#include <blShader.h>
#include <Resources/blResource.h>

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
}

BL_RESOURCE(blMaterial, eResourceType::Material);