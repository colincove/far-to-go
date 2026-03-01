#include <blMaterial.h>

namespace BoulderLeaf::Graphics
{
	blMaterial::blMaterial(blShaderResourcePtr shader, bool instanced)
		: shader(shader), mInstanced(instanced)
	{
	}
}