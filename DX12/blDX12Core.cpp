#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{

	//NOTE: This format was randomly chosen. Verify correct value later
	const DXGI_FORMAT Constants::BackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	const DXGI_FORMAT Constants::DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	const int Constants::SwapChainBufferCount = 2;
	const int Constants::SrvHeapSize = 64;

	DXGI_FORMAT ConvertImageFormatToDXGIFormat(eImageFormat format)
	{
		return (DXGI_FORMAT) format;
	}

	D3D12_INPUT_CLASSIFICATION ConvertInputCassificationToD3D12InputClassification(eInputClassification format)
	{
		return (D3D12_INPUT_CLASSIFICATION)format;
	}

	D3D12_INPUT_ELEMENT_DESC ConvertInputElementDescToD3D12InputElementDesc(const InputElementDesc& desc)
	{
		D3D12_INPUT_ELEMENT_DESC d3dDesc;
		d3dDesc.SemanticName = desc.SemanticName.c_str();
		d3dDesc.SemanticIndex = desc.SemanticIndex;
		d3dDesc.Format = ConvertImageFormatToDXGIFormat(desc.Format);
		d3dDesc.InputSlot = desc.InputSlot;
		d3dDesc.AlignedByteOffset = desc.AlignedByteOffset;
		d3dDesc.InputSlotClass = ConvertInputCassificationToD3D12InputClassification(desc.InputSlotClass);
		d3dDesc.InstanceDataStepRate = desc.InstanceDataStepRate;
		return d3dDesc;
	}
}