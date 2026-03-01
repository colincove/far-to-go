#pragma once

#include <blDX12Core.h>
#include <d3d12.h>
#include <vector>

namespace BoulderLeaf::Graphics::DX12
{
	class blPSO
	{
	private:
		ComPtr<ID3D12PipelineState> mPSO = nullptr;
		std::size_t mHash;
	public:
		blPSO(ComPtr<ID3D12Device8> device,
			ComPtr<ID3D12RootSignature> rootSignature,
			std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout,
			ComPtr<ID3DBlob> mvsByteCode,
			ComPtr<ID3DBlob> mpsByteCode);

		ComPtr<ID3D12PipelineState> GetDX12PSO() const { return mPSO; }

		friend bool operator==(const blPSO& lhs, const blPSO& rhs) 
		{
			return lhs.mHash == rhs.mHash; // OK: can access private member
		}

		friend bool operator!=(const blPSO& lhs, const blPSO& rhs) { return !(lhs == rhs); }
	};
}