#include <blDX12ShaderCache.h>
#include <blDX12ReadFile.h>
#include <format>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blShaderCache::blShaderCache(std::shared_ptr<blDevice> device)
		: mDevice(device), blDX12ResourceDataCache<blShaderCacheData, blShaderResource>()
	{

	}

	void blShaderCache::InitializeCache(
		const blShaderResource& resource,
		blShaderCacheData& cache)
	{
		const blShader& shader = resource.GetData();
		cache.RootSignature = std::make_unique<blRootSignature>(mDevice, shader);
		const std::wstring shaderNameW = std::wstring(shader.mName.begin(), shader.mName.end());
		cache.mvsByteCode = LoadBinary(std::format(L"../../shaders/bin/v_{}.cso", shaderNameW));
		cache.mpsByteCode = LoadBinary(std::format(L"../../shaders/bin/p_{}.cso", shaderNameW));
		cache.InputLayout = BuildInputLayoutDesc(shader.mLayout);
	}
}