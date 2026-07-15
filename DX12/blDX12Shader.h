#pragma once

#include <blShader.h>
#include <vector>
#include <memory>
#include <blDevice.h>
#include <blRootSignature.h>

namespace BoulderLeaf
{
	template<typename T>
	class blResourceHandleOfType;
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ShaderData
	{
		blShader Shader;
		std::unique_ptr<blRootSignature> RootSignature;
		ComPtr<ID3DBlob> mvsByteCode = nullptr;
		ComPtr<ID3DBlob> mpsByteCode = nullptr;
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputLayout;
	};

	void LoadShaderData(const blShader& shader, 
		blDevice* device,
		blDX12ShaderData& data);

	void LoadShaderDataFromResource_exp(
		const blResourceHandleOfType<blShaderResource_exp> shader, 
		blDevice* device,
		blDX12ShaderData& data);
}
