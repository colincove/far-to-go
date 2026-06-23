#include <blBuffer.h>
#include <format>

namespace BoulderLeaf::Graphics
{
	void MarshalBuffer(
		size_t elementCount, 
		const std::vector<BufferElementDescription>& elementDescriptions,
		std::function<const byte*(size_t)> getSourcePointerByIndex,
		std::function<byte*(size_t)> getDestinationPointerByIndex,
		const blBufferElementAdapter& adapter)
	{
		for (size_t i = 0; i < elementCount; ++i)
		{
			byte* destVertex = static_cast<byte*>(getDestinationPointerByIndex(i));
			const byte* srcVertex = static_cast<const byte*>(getSourcePointerByIndex(i));
			
			MarshalBufferElement(
				elementDescriptions,
				destVertex,
				srcVertex,
				adapter
			);
		}
	}

	void MarshalBufferElement(
		const std::vector<BufferElementDescription>& elementDescriptions,
		byte* destVertex,
		const byte* srcVertex,
		const blBufferElementAdapter& adapter)
	{
		size_t srcOffset = 0;
		for (const BufferElementDescription& elementDescription : elementDescriptions)
		{
			switch (elementDescription.ElementType)
			{
			case BufferElementType::Float2:
			{
				const Math::Vector2& srcElement = *reinterpret_cast<const Math::Vector2*>(srcVertex + srcOffset);
				adapter.MarshalVector2(srcElement, destVertex);
				srcOffset += sizeof(Math::Vector2);
				break;
			}
			case BufferElementType::Float3:
			{
				const Math::Vector3& srcElement = *reinterpret_cast<const Math::Vector3*>(srcVertex + srcOffset);
				adapter.MarshalVector3(srcElement, destVertex);
				srcOffset += sizeof(Math::Vector3);
				break;
			}
			case BufferElementType::Float4:
			{
				const Math::Vector4& srcElement = *reinterpret_cast<const Math::Vector4*>(srcVertex + srcOffset);
				adapter.MarshalVector4(srcElement, destVertex);
				srcOffset += sizeof(Math::Vector4);
				break;
			}
			case BufferElementType::Matrix3x3:
			{
				const Math::Matrix3x3& srcElement = *reinterpret_cast<const Math::Matrix3x3*>(srcVertex + srcOffset);
				adapter.MarshalMatrix3x3(srcElement, destVertex);
				srcOffset += sizeof(Math::Matrix3x3);
				break;
			}
			case BufferElementType::Matrix4x4:
			{
				const Math::Matrix4x4& srcElement = *reinterpret_cast<const Math::Matrix4x4*>(srcVertex + srcOffset);
				adapter.MarshalMatrix4x4(srcElement, destVertex);
				srcOffset += sizeof(Math::Matrix4x4);
				break;
			}
			default:
				throw std::runtime_error(std::format("Unknown vertex element type {}", static_cast<int>(elementDescription.ElementType)));
			}

			destVertex += adapter.SizeOfElement(elementDescription.ElementType);
		}
	}

	size_t GetBufferElementSize(
		const std::vector<BufferElementDescription>& elementDescriptions,
		const blBufferElementAdapter& adapter)
	{
		size_t elementSize = 0;
		for (const BufferElementDescription& elementDescription : elementDescriptions)
		{
			elementSize += adapter.SizeOfElement(elementDescription.ElementType);
		}

		return elementSize;
	}

	blDataElementBuffer::blDataElementBuffer(BufferFormat format,
		std::vector<BufferElementDescription> elementDescriptions,
		std::unique_ptr<byte[]> data)
		: mFormat(format),
		mElementDescriptions(elementDescriptions),
		mData(std::move(data))
	{

	}

	blVertexBufferWithPassBuffer::blVertexBufferWithPassBuffer(
		blDataElementBufferResourcePtr passElementBuffer,
		blDataBufferInterfaceResourcePtr vertexBuffer)
		: mPassElementBuffer(passElementBuffer),
		mVertexBuffer(vertexBuffer)
	{
	}
}