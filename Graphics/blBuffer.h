#pragma once

#include <blVector2.inl>
#include <blVector3.inl>
#include <blVector4.inl>
#include <blMatrix2x2.inl>
#include <blMatrix2x3.inl>
#include <blMatrix3x3.inl>
#include <blMatrix4x4.inl>
#include <exception>
#include <vector>
#include <cstdint>
#include <blTypes.h>
#include <functional>
#include <stdexcept>
#include <string>
#include <blGraphicsCore.h>
#include <minwindef.h>
#include <Resources/blResources.h>

namespace BoulderLeaf {
	template<typename TResource>
	class blResourceHandleOfType;
}

namespace BoulderLeaf::Graphics
{
	enum class BufferFormat
	{
		BoulderLeaf,
		DX12
	};

	enum class BufferElementType : int
	{
		Float,
		Float2,
		Float3,
		Float4,
		Matrix3x3,
		Matrix4x4,
		Bool,
		Int,
		UInt,
		Half,
		Double,
	};

	struct BufferElementDescription
	{
		std::string Name;
		BufferElementType ElementType;
	};

	struct BufferDescription
	{
		BufferFormat format;
		//memory ownership ambiguos. use unique_ptr?
		std::vector<BufferElementDescription> elementDescriptions;
		uint64_t minStride;
	};

	struct blBufferElementDescriptionResource : public blBaseResource
	{
		BufferElementType mElementType;
		blStringResource mNameStringResource;

		blBufferElementDescriptionResource(blResourceStream& stream, const BufferElementDescription& element);

		const std::string_view GetName() const;
	};

	struct blBufferDescriptionResource : public blListDynamicResource<blBufferElementDescriptionResource, BufferElementDescription>
	{
		BufferFormat mFormat;
		uint64_t mMinStride;

		blBufferDescriptionResource(
			blResourceStream& stream,
			const BufferDescription& desc
		);

		BufferDescription GetBufferDescription() const;
	};

	template<typename TElement, BufferFormat TFormat>
	struct BufferDefinitionTemplate
	{
		constexpr static BufferFormat Format = TFormat;
		using ElementType = TElement;
		static const std::vector<BufferElementDescription> Description;
	};

	struct blArrayBufferResource : public blBaseResource
	{
		blResourceRefOfType<blBufferDescriptionResource> mDescriptionResourceRef;
		blResourceRefOfType<blListResource> mBufferResourceRef;

		blArrayBufferResource(
			blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
			blResourceRefOfType<blListResource> bufferResourceRef
		);
	};

	class blBufferElementAdapter
	{
	public:
		virtual size_t SizeOfElement(BufferElementType Type) const = 0;
		virtual void MarshalFloat(const float& srcElement, byte* destElement) const = 0;
		virtual void MarshalVector2(const Math::Vector2& srcElement, byte* destElement) const = 0;
		virtual void MarshalVector3(const Math::Vector3& srcElement, byte* destElement) const = 0;
		virtual void MarshalVector4(const Math::Vector4& srcElement, byte* destElement) const = 0;
		virtual void MarshalMatrix3x3(const Math::Matrix3x3& srcElement, byte* destElement) const = 0;
		virtual void MarshalMatrix4x4(const Math::Matrix4x4& srcElement, byte* destElement) const = 0;
		virtual void MarshalBool(const bool& srcElement, byte* destElement) const = 0;
		virtual void MarshalInt(const int32_t& srcElement, byte* destElement) const = 0;
		virtual void MarshalUInt(const uint32_t& srcElement, byte* destElement) const = 0;
		virtual void MarshalHalf(const uint16_t& srcElement, byte* destElement) const = 0;
		virtual void MarshalDouble(const double& srcElement, byte* destElement) const = 0;
		virtual BufferFormat GetFormat() const = 0;

		uint64_t SizeOf(const BufferDescription& descriptions) const;
	};

	void MarshalBuffer(
		size_t elementCount,
		const std::vector<BufferElementDescription>& elementDescriptions,
		std::function<const byte* (size_t)> getSourcePointerByIndex,
		std::function<byte* (size_t)> getDestinationPointerByIndex,
		const blBufferElementAdapter& adapter);

	void MarshalBufferElement(
		const std::vector<BufferElementDescription>& elementDescriptions,
		byte* destVertex,
		const byte* srcVertex,
		const blBufferElementAdapter& adapter);

	size_t GetBufferElementSize(
		const std::vector<BufferElementDescription>& elementDescriptions,
		const blBufferElementAdapter& adapter);

	blResourceHandleOfType<blArrayBufferResource> CreateArrayBufferResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		const void* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount);

	blResourceHandleOfType<blArrayBufferResource> CreateArrayBufferResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		uint64_t vertexSize,
		uint32_t vertexCount);

	template<typename TElement>
	blResourceHandleOfType<blArrayBufferResource> CreateArrayBufferResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		const TElement* vertices,
		uint32_t vertexCount)
	{
		return CreateArrayBufferResource(
			resourceContainer,
			name,
			descriptionResourceRef,
			vertices,
			sizeof(TElement),
			vertexCount);
	}

	blResourceHandleOfType<blArrayBufferResource> CreateArrayBufferResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		const blArrayBufferResource& sourceArray,
		const blBufferElementAdapter& adapter);

	struct blConstantBufferResource : blBaseResource
	{
		blResourceRefOfType<blArrayBufferResource> mInstanceConstantBuffer;
		blListResource mPassConstantBuffers;

		blConstantBufferResource(blResourceStream& stream,
			blResourceRefOfType<blArrayBufferResource> instanceConstantBuffer,
			std::vector<blResourceRefOfType<blArrayBufferResource>> passConstantBuffers);
	};
}