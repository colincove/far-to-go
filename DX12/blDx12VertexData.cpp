#include <blDx12VertexData.h>

namespace BoulderLeaf::Graphics
{
	template<>
	DX12::Vertex Graphics::From<StandardVertex, DX12::Vertex>(const StandardVertex& from)
	{
		return DX12::Vertex(
			XMFLOAT3(from.Position.x, from.Position.y, from.Position.z),
			XMFLOAT3(from.Normal.x, from.Normal.y, from.Normal.z),
			XMFLOAT3(from.Tangent.x, from.Tangent.y, from.Tangent.z),
			XMFLOAT4(from.Colour.x, from.Colour.y, from.Colour.z, from.Colour.w),
			XMFLOAT2(from.UV.x, from.UV.y)
		);
	}
}