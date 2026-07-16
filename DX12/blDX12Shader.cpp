#include <blDX12Shader.h>
#include <blDX12ReadFile.h>
#include <format>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	void LoadShaderData(
		const blShader& shader,
		blDevice* device,
		blDX12ShaderData& data)
	{
		data.RootSignature = std::make_unique<blRootSignature>(device, shader);
		const std::wstring shaderNameW = std::wstring(shader.mName.begin(), shader.mName.end());
		data.mvsByteCode = LoadBinary(std::format(L"../../shaders/bin/v_{}.cso", shaderNameW));
		data.mpsByteCode = LoadBinary(std::format(L"../../shaders/bin/p_{}.cso", shaderNameW));
		data.InputLayout = BuildInputLayoutDesc(shader.mLayout);
	}

	void LoadShaderDataFromResource_exp(
		const blResourceHandleOfType<blShaderResource> shaderResource, 
		blDevice* device,
		blDX12ShaderData& data)
	{
		data.Shader = ShaderResourceToShader(shaderResource);
		data.RootSignature = std::make_unique<blRootSignature>(device, data.Shader);
		const std::wstring shaderNameW = std::wstring(shaderResource->mNameStringResource.GetStringView().begin(), shaderResource->mNameStringResource.GetStringView().end());
		data.mvsByteCode = LoadBinary(std::format(L"../../shaders/bin/v_{}.cso", shaderNameW));
		data.mpsByteCode = LoadBinary(std::format(L"../../shaders/bin/p_{}.cso", shaderNameW));
		data.InputLayout = BuildInputLayoutDesc(data.Shader.mLayout);
	}
}