#pragma once

#include <blShader.h>
#include <blPSO.h>
#include <vector>
#include <memory>
#include <blDevice.h>
#include <blDX12ResourceDataCache.h>
#include <blDX12ShaderCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blPSOCacheData
	{
		std::shared_ptr<blPSO> PSO;
	};

	class blPSOCache : public blDX12ResourceDataCache<blPSOCacheData, blShaderResource>
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blShaderCache> mShaderCache;
	public:
		blPSOCache(std::shared_ptr<blDevice> device, std::shared_ptr<blShaderCache> shaderCache);
		virtual void InitializeCache(
			const blShaderResource& resource,
			blPSOCacheData& cache) override;
	};
}