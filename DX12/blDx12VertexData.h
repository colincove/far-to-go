#pragma once

#include <blDX12Core.h>
#include <blDX12Math.inl>
#include <d3d12.h>
#include <DirectXMath.h>
#include <blMesh.h>

using namespace DirectX;

namespace BoulderLeaf::Graphics::DX12
{
	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = Math::Identity4x4();
	};

	struct Vertex
	{
	public:
		XMFLOAT3 Pos;
		XMFLOAT3 Norm;
		XMFLOAT4 Color;
		XMFLOAT2 UV;
	};

	using Mesh = Graphics::blMesh<Vertex>;

	inline bool operator==(const Vertex& lhs, const Vertex& rhs)
	{
		return memcmp(&lhs, &rhs, sizeof(Vertex));
	}

	const std::vector<D3D12_INPUT_ELEMENT_DESC> VertexDesc =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	/*static inline CD3DX12_RESOURCE_DESC Buffer(
	UINT64 width,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		UINT64 alignment = 0)
	{
		return CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER,
			alignment, width, 1, 1, 1,
			DXGI_FORMAT_UNKNOWN, 1, 0,
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}*/
}

namespace BoulderLeaf::Graphics
{
	/*template<>
	DX12::Vertex Graphics::From<StandardVertex, DX12::Vertex>(StandardVertex vertex)
	{
		return {
			XMFLOAT3(),
			XMFLOAT3(),
			XMFLOAT4(),
			XMFLOAT2()
		};
	}*/
}