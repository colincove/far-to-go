#pragma once

#include <blBuffer.h>
#include <d3d12.h>

namespace BoulderLeaf::Graphics::DX12
{
	class DX12BufferAdapter : public blBufferElementAdapter
	{
	public:
		static const DX12BufferAdapter& Get();
		virtual size_t SizeOfElement(BufferElementType Type) const override;
		virtual void MarshalFloat(const float& srcElement, byte* destElement) const override;
		virtual void MarshalBool(const bool& srcElement, byte* destElement) const override;
		virtual void MarshalInt(const int32_t& srcElement, byte* destElement) const override;
		virtual void MarshalUInt(const uint32_t& srcElement, byte* destElement) const override;
		virtual void MarshalHalf(const uint16_t& srcElement, byte* destElement) const override;
		virtual void MarshalDouble(const double& srcElement, byte* destElement) const override;
		virtual void MarshalVector2(const BoulderLeaf::Math::Vector2& srcElement, byte* destElement) const override;
		virtual void MarshalVector3(const BoulderLeaf::Math::Vector3& srcElement, byte* destElement) const override;
		virtual void MarshalVector4(const BoulderLeaf::Math::Vector4& srcElement, byte* destElement) const override;
		virtual void MarshalMatrix3x3(const BoulderLeaf::Math::Matrix3x3& srcElement, byte* destElement) const override;
		virtual void MarshalMatrix4x4(const BoulderLeaf::Math::Matrix4x4& srcElement, byte* destElement) const override;
		virtual BufferFormat GetFormat() const override;
	};

	std::vector<D3D12_INPUT_ELEMENT_DESC> BuildInputLayoutDesc(
		const std::vector<BufferElementDescription>& elementDescriptions);
}