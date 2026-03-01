#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	const std::vector<BufferElementDescription> blStandardObjectConstantsDefinition::Description =
	{
		{ "WorldViewProj", BufferElementType::Matrix4x4 }
	};
}