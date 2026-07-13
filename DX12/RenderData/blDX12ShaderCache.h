#pragma once

#include <blShader.h>
#include <vector>
#include <memory>
#include <blDevice.h>
#include <blRootSignature.h>
#include <blDX12ResourceDataCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blShaderCacheData
	{
		std::unique_ptr<blRootSignature> RootSignature;
		ComPtr<ID3DBlob> mvsByteCode = nullptr;
		ComPtr<ID3DBlob> mpsByteCode = nullptr;
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout;
	};

	class blShaderCache : public blDX12ResourceDataCache<blShaderCacheData, blShaderResource>
	{
	private:
		blDevice* mDevice;
	public:
		blShaderCache(blDevice* device);
		virtual void InitializeCache(
			const blShaderResource& resource,
			blShaderCacheData& cache) override;
	};
}