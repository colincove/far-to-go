#include <blDX12Core.h>

namespace BoulderLeaf::Graphics::DX12
{	
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