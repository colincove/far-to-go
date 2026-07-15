#include <blMaterial.h>

namespace BoulderLeaf::Graphics
{
	blMaterial::blMaterial(blShaderResourcePtr shader, bool instanced)
		: shader(shader), mInstanced(instanced)
	{
	}

	blMaterialResource_exp::blMaterialResource_exp(
		blResourceRefOfType<blShaderResource_exp> shaderResourceRef,
		bool instanced)
		: mInstanced(instanced), mShaderResourceRef(shaderResourceRef)
	{
	}
}