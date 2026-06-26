#include <blRootSignature.h>

namespace BoulderLeaf::Graphics::DX12
{
	blRootSignature::blRootSignature(
		std::shared_ptr<blDevice> device,
		const blShader& shader)
	{
		// Shader programs typically require resources as input (constant
		// buffers, textures, samplers). The root signature defines the
		// resources the shader programs expect. If we think of the shader
		// programs as a function, and the input resources as function
		// parameters, then the root signature can be thought of as defining
		// the function signature.
		// Root parameter can be a table, root descriptor or root constants.
		std::vector<CD3DX12_ROOT_PARAMETER> slotRootParameter;
		slotRootParameter.resize(shader.mParameters.size());

		std::vector<CD3DX12_DESCRIPTOR_RANGE> cbvDescRanges;
		cbvDescRanges.reserve(shader.mParameters.size());

		for (int i = 0; i < shader.mParameters.size(); ++i)
		{
			const blShader::Parameter parameter = shader.mParameters[i];

			switch (parameter.type)
			{
				case blShader::Parameter::ConstantBuffer:
				{
					cbvDescRanges.push_back(CD3DX12_DESCRIPTOR_RANGE());
					CD3DX12_DESCRIPTOR_RANGE& cbvTable = cbvDescRanges[cbvDescRanges.size()-1];
					cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, parameter.Count, parameter.Register, parameter.Space);
					slotRootParameter[i].InitAsDescriptorTable(1, &cbvTable);
					break;
				}
				case blShader::Parameter::ShaderResource:
				{
					CD3DX12_DESCRIPTOR_RANGE srvTable;
					srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, parameter.Count, parameter.Register, parameter.Space);
					slotRootParameter[i].InitAsDescriptorTable(1, &srvTable);
					break;
				}
				case blShader::Parameter::UnorderedAccess:
				{
					CD3DX12_DESCRIPTOR_RANGE uavTable;
					uavTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, parameter.Count, parameter.Register, parameter.Space);
					slotRootParameter[i].InitAsDescriptorTable(1, &uavTable);
					break;
				}
				case blShader::Parameter::Sampler:
				{
					CD3DX12_DESCRIPTOR_RANGE samplerTable;
					samplerTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, parameter.Count, parameter.Register, parameter.Space);
					slotRootParameter[i].InitAsDescriptorTable(1, &samplerTable);
					break;
				}
			}
		}

		// A root signature is an array of root parameters.
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(static_cast<UINT>(slotRootParameter.size()), slotRootParameter.data(), 0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		// create a root signature with a single slot which points to a
		// descriptor range consisting of a single constant buffer
		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		DX12_API_CALL(D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf()));

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}

		DX12_API_CALL(device->GetDX12Device()->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(&mRootSignature)));

		std::wstring stemp = L"[BL] " + std::wstring(shader.mName.begin(), shader.mName.end());
		DX12_API_CALL(mRootSignature->SetName(stemp.c_str()));
	}
}