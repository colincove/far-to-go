#include <blDX12PSODataCache.h>
#include <format>

namespace BoulderLeaf::Graphics::DX12
{
	blPSOCache::blPSOCache(blDevice* device, blShaderCache* shaderCache)
		: mDevice(device), mShaderCache(shaderCache), blDX12ResourceDataCache<blPSOCacheData, blShaderResource>()
	{

	}

	void blPSOCache::InitializeCache(
		const blShaderResource& shader,
		blPSOCacheData& cache)
	{
		const blShaderCacheData& shaderCacheData = mShaderCache->Get(shader);
		cache.PSO = std::make_shared<blPSO>(
			mDevice->GetDX12Device(),
			shaderCacheData.RootSignature->GetRootSignature(),
			shaderCacheData.InputLayout,
			shaderCacheData.mvsByteCode,
			shaderCacheData.mpsByteCode,
			shader.GetName());
	}
}