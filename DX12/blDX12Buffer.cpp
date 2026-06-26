#include <blDX12Buffer.h>
#include <DirectXMath.h>
#include <format>

namespace BoulderLeaf::Graphics::DX12
{
	const DX12BufferAdapter& DX12BufferAdapter::Get()
	{
		static DX12BufferAdapter gDX12BufferAdapter;
		return gDX12BufferAdapter;
	}

	void DX12BufferAdapter::MarshalFloat(const float& srcElement, byte* destElementPtr) const
	{
		float& destElement = *reinterpret_cast<float*>(destElementPtr);
		destElement = srcElement;
	}

	void DX12BufferAdapter::MarshalVector2(const Math::Vector2& srcElement, byte* destElementPtr) const
	{
		DirectX::XMFLOAT2& destElement = *reinterpret_cast<DirectX::XMFLOAT2*>(destElementPtr);
		destElement = DirectX::XMFLOAT2(srcElement.data);
	}

	void DX12BufferAdapter::MarshalVector3(const Math::Vector3& srcElement, byte* destElementPtr) const
	{
		DirectX::XMFLOAT3& destElement = *reinterpret_cast<DirectX::XMFLOAT3*>(destElementPtr);
		destElement = DirectX::XMFLOAT3(srcElement.data);
	}

	void DX12BufferAdapter::MarshalVector4(const Math::Vector4& srcElement, byte* destElementPtr) const
	{
		DirectX::XMFLOAT4& destElement = *reinterpret_cast<DirectX::XMFLOAT4*>(destElementPtr);
		destElement = DirectX::XMFLOAT4(srcElement.data);
	}

	void DX12BufferAdapter::MarshalMatrix3x3(const Math::Matrix3x3& srcElement, byte* destElementPtr) const
	{
		DirectX::XMFLOAT3X3& destElement = *reinterpret_cast<DirectX::XMFLOAT3X3*>(destElementPtr);
		destElement = DirectX::XMFLOAT3X3(srcElement.elements);
	}

	void DX12BufferAdapter::MarshalMatrix4x4(const Math::Matrix4x4& srcElement, byte* destElementPtr) const
	{
		DirectX::XMFLOAT4X4& destElement = *reinterpret_cast<DirectX::XMFLOAT4X4*>(destElementPtr);
		destElement = DirectX::XMFLOAT4X4(srcElement.elements);
	}

	BufferFormat DX12BufferAdapter::GetFormat() const 
	{
		return BufferFormat::DX12;
	}

	size_t DX12BufferAdapter::SizeOfElement(BufferElementType type) const
	{
		switch (type)
		{
		case BufferElementType::Float:
			return sizeof(float);
		case BufferElementType::Float2:
			return sizeof(DirectX::XMFLOAT2);
		case BufferElementType::Float3:
			return sizeof(DirectX::XMFLOAT3);
		case BufferElementType::Float4:
			return sizeof(DirectX::XMFLOAT4);
		case BufferElementType::Matrix3x3:
			return sizeof(DirectX::XMFLOAT3X3);
		case BufferElementType::Matrix4x4:
			return sizeof(DirectX::XMFLOAT4X4);
		}

		assert(false);
		return 0;
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> BuildInputLayoutDesc(
		const std::vector<BufferElementDescription>& elementDescriptions)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc;
		inputLayoutDesc.reserve(elementDescriptions.size());

		for (const BufferElementDescription& element : elementDescriptions)
		{
			DXGI_FORMAT format;
			D3D12_INPUT_CLASSIFICATION inputClassification = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

			switch (element.ElementType)
			{
			case BufferElementType::Float:
				format = DXGI_FORMAT_R32_FLOAT;
			case BufferElementType::Float2:
				format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case BufferElementType::Float3:
				format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case BufferElementType::Float4:
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				break;
			default:
				throw std::runtime_error(std::format("Unknown vertex element type {}", static_cast<int>(element.ElementType)));
			}
			inputLayoutDesc.push_back({ element.Name.c_str(), 0, format, 0, 0, inputClassification, 0 });
		}
		return inputLayoutDesc;
	}
}