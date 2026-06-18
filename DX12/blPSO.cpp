#include <blPSO.h>
#include <blDX12Core.h>
#include <vector>
#include <directx\d3dx12_core.h>

namespace BoulderLeaf::Graphics::DX12
{
	blPSO::blPSO(ComPtr<ID3D12Device8> device,
		ComPtr<ID3D12RootSignature> rootSignature,
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout,
		ComPtr<ID3DBlob> mvsByteCode,
		ComPtr<ID3DBlob> mpsByteCode,
		std::wstring name)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
		psoDesc.pRootSignature = rootSignature.Get();
		psoDesc.VS =
		{
			reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
			mvsByteCode->GetBufferSize()
		};
		psoDesc.PS =
		{
			reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
			mpsByteCode->GetBufferSize()
		};
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = Constants::BackbufferFormat;
		//The render target sample desc in slot 0 does not match that specified by the current pipeline state.
		//psoDesc.SampleDesc.Count = m_dx12->m4xMsaaState ? 4 : 1;
		//psoDesc.SampleDesc.Quality = m_dx12->m4xMsaaState ? (m_dx12->m4xMsaaQuality - 1) : 0;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		psoDesc.DSVFormat = Constants::DepthStencilFormat;
		device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));

		//TODO: Find better way to hash PSO to manage caches and generating of PSOs
		std::size_t h1 = std::hash<int>{}(mvsByteCode->GetBufferSize());
		std::size_t h2 = std::hash<int>{}(mpsByteCode->GetBufferSize());
		mHash = h1 ^ (h2 << 1);

		std::wstring wName = L"[BL] " + name;
		mPSO->SetName(wName.c_str());
	}
}