#pragma once

#include <array>
#include <blDX12.h>
#include <blDx12VertexData.h>
#include "AbstractExample.h"
#include <blDX12MeshGeometry.h>
#include <memory>
#include "blDX12UploadBuffer.h"

namespace BoulderLeaf::Graphics::DX12
{
	class BoxExample : public AbstractExample
	{
		struct ObjectConstants
		{
			XMFLOAT4X4 WorldViewProj = XMFLOAT4X4_Identity();
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
		std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
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
}