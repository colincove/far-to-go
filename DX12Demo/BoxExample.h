#pragma once

#include <array>
#include <blDX12.h>
#include "AbstractExample.h"
#include <blDX12MeshGeometry.h>
#include <memory>
#include "blDX12UploadBuffer.h"

namespace BoulderLeaf::Graphics::DX12
{
	class BoxExample : public AbstractExample
	{
	public:
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
		using BoxMeshGeometry = MeshGeometry<Mesh>;

		const std::vector<D3D12_INPUT_ELEMENT_DESC> VertexDesc =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"Color", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
		};

	private:
		void OnMouseDown(WPARAM btnState, int x, int y);
		void OnMouseUp(WPARAM btnState, int x, int y);
		void OnMouseMove(WPARAM btnState, int x, int y);

		void BuildConstantBuffers();
		void BuildRootSignature();
		void BuildShadersAndInputLayout();
		void BuildBoxGeometry();
		void BuildPSO();
	private:
		ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
		std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
		std::unique_ptr<BoxMeshGeometry> mBoxGeo = nullptr;
		ComPtr<ID3DBlob> mvsByteCode = nullptr;
		ComPtr<ID3DBlob> mpsByteCode = nullptr;
		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
		ComPtr<ID3D12PipelineState> mPSO = nullptr;
		XMFLOAT4X4 mWorld = XMFLOAT4X4_Identity();
		XMFLOAT4X4 mView = XMFLOAT4X4_Identity();
		float mTheta = 1.5f * XM_PI;
		float mPhi = XM_PIDIV4;
		float mRadius = 5.0f;
		POINT mLastMousePos;
		ComPtr<ID3D12Resource> mUploadCBuffer;
	public:
		BoxExample(std::shared_ptr<DX12> dx12);
		~BoxExample();
	public:
		void Update(const Metrics::blTime& gameTime) override;
		void Draw() override;
	};

	inline bool operator==(const BoxExample::Vertex& lhs, const BoxExample::Vertex& rhs)
	{
		return memcmp(&lhs, &rhs, sizeof(BoxExample::Vertex));
	}
}