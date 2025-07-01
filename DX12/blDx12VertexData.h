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
		XMFLOAT3 Tan;
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
		{"Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
}