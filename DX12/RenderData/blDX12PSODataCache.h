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
		blDevice* mDevice;
		blShaderCache* mShaderCache;
	public:
		blPSOCache(blDevice* device, blShaderCache* shaderCache);
		virtual void InitializeCache(
			const blShaderResource& resource,
			blPSOCacheData& cache) override;
	};
}