#pragma once

#include <blVector3.inl>
#include <blVector2.inl>
#include <blVector4.inl>
#include <blBuffer.h>

namespace BoulderLeaf::Graphics
{
	struct StandardVertex
	{
		Math::Vector3 Position;
		Math::Vector3 Normal;
		Math::Vector3 Tangent;
		Math::Vector4 Colour;
		Math::Vector2 UV;
	};

	using VertexElementType = BufferElementType;
	using VertexElementDescription = BufferElementDescription;

	template<typename TVertex, BufferFormat TFormat>
	struct VertexDefinition : public BufferDefinitionTemplate<TVertex, TFormat>
	{
		using VertexType = TVertex;
	};

	using StandardVertexDefinition = VertexDefinition<StandardVertex, BufferFormat::BoulderLeaf>;
}