
#include <blVertex.h>

namespace BoulderLeaf::Graphics
{
	const std::vector<VertexElementDescription> StandardVertexDefinition::Description =
	{
		{ "Position",	VertexElementType::Float3 },
		{ "Normal",		VertexElementType::Float3 },
		{ "Tangent",	VertexElementType::Float3 },
		{ "Color",		VertexElementType::Float4 },
		{ "TEXCOORD",	VertexElementType::Float2 }
	};
}