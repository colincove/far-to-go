#include <blMaterial.h>

namespace BoulderLeaf::Graphics
{
	blMaterialResource::blMaterialResource(
		blResourceRefOfType<blShaderResource> shaderResourceRef,
		bool instanced)
		: mInstanced(instanced), mShaderResourceRef(shaderResourceRef)
	{
	}
}