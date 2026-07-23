
#include <blVertex.h>

namespace BoulderLeaf::Graphics
{
	const std::vector<VertexElementDescription> StandardVertexDefinition::Description =
	{
		{ "Position",	VertexElementType::Float3, BufferElementSemantic::Position },
		{ "Normal",		VertexElementType::Float4, BufferElementSemantic::Normal },
		{ "Tangent",	VertexElementType::Float3, BufferElementSemantic::Tangent },
		{ "Color",		VertexElementType::Float4, BufferElementSemantic::Colour },
		{ "TEXCOORD",	VertexElementType::Float2, BufferElementSemantic::TEXCOORD }
	};
}