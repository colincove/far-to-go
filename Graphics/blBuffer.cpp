#include <blBuffer.h>
#include <format>
#include <cstring>
#include <cassert>
#include <Resources/blResourcesExprimental.h>
#include <Resources/blResourceHandleOfType.h>

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
			case BufferElementType::Float:
			{
				const float& srcElement = *reinterpret_cast<const float*>(srcVertex + srcOffset);
				adapter.MarshalFloat(srcElement, destVertex);
				srcOffset += sizeof(float);
				break;
			}
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

	uint64_t blBufferElementAdapter::SizeOf(const BufferDescription& description) const
	{
		assert(GetFormat() == description.format);
		return GetBufferElementSize(description.elementDescriptions, *this);
	}

	blVertexBufferWithPassBuffer::blVertexBufferWithPassBuffer(
		blDataElementBufferResourcePtr passElementBuffer,
		blDataBufferInterfaceResourcePtr vertexBuffer)
		: mPassElementBuffer(passElementBuffer),
		mVertexBuffer(vertexBuffer)
	{
	}

	uint64_t InlineBufferDescription::CalculateBufferElementDescriptionOffset()
	{
		return sizeof(InlineBufferDescription);
	}

	uint64_t InlineBufferDescription::CalculateTotalSize(std::vector<BufferElementDescription> elementDescriptions)
	{
		uint64_t totalSize = CalculateBufferElementDescriptionOffset();

		for (const BufferElementDescription& desc : elementDescriptions)
		{
			InlineBufferElementDescription inlineDesc;
			inlineDesc.mHeader.ElementType = desc.ElementType;
			inlineDesc.mHeader.nameStringLength = (uint32_t)desc.Name.length();
			totalSize += inlineDesc.CalculateTotalSize();
		}

		return totalSize;
	}

	void InlineBufferDescription::WriteDescription(const BufferDescription& desc)
	{
		mHeader.format = desc.format;
		mHeader.numElementDescriptions = desc.elementDescriptions.size();

		uint64_t writeOffset = CalculateBufferElementDescriptionOffset();

		for (const BufferElementDescription& d : desc.elementDescriptions)
		{
			InlineBufferElementDescription& elemDesc = reinterpret_cast<InlineBufferElementDescription&>(
				*(reinterpret_cast<char*>(this) + writeOffset)
			);

			elemDesc.mHeader.ElementType = d.ElementType;
			elemDesc.mHeader.nameStringLength = d.Name.length();
			writeOffset += InlineBufferElementDescription::CalculateNameStringOffset();
			const uint64_t nameSize = d.Name.length() * sizeof(char);
			memcpy(reinterpret_cast<char*>(this) + writeOffset, d.Name.c_str(), nameSize);
			writeOffset += nameSize;
		}
	}

	// IterateElements removed; use Iterator for element traversal

	InlineBufferDescription::Iterator::Iterator(InlineBufferElementDescription* begin, int numElementDescriptions)
		: mNumElementDescriptions(numElementDescriptions),
		mHead(reinterpret_cast<byte*>(begin)),
		mBegin(mHead)
	{

	}

	InlineBufferDescription::Iterator::Iterator(InlineBufferDescription& desc)
		: mNumElementDescriptions(static_cast<int>(desc.mHeader.numElementDescriptions)),
		mHead(reinterpret_cast<byte*>(reinterpret_cast<char*>(&desc) + InlineBufferDescription::CalculateBufferElementDescriptionOffset())),
		mBegin(mHead)
	{

	}

	BufferDescription InlineBufferDescription::ToBufferDescription() const
	{
		BufferDescription desc;
		desc.format = mHeader.format;
		desc.elementDescriptions.reserve(mHeader.numElementDescriptions);

		uint64_t readOffset = CalculateBufferElementDescriptionOffset();

		for (unsigned int i = 0; i < mHeader.numElementDescriptions; ++i)
		{
			const InlineBufferElementDescription& elemDesc = reinterpret_cast<const InlineBufferElementDescription&>(
				*(reinterpret_cast<const char*>(this) + readOffset)
			);

			BufferElementDescription be;
			be.ElementType = elemDesc.mHeader.ElementType;
			const char* namePtr = reinterpret_cast<const char*>(this) + readOffset + InlineBufferElementDescription::CalculateNameStringOffset();
			be.Name.assign(namePtr, elemDesc.mHeader.nameStringLength);

			desc.elementDescriptions.push_back(std::move(be));

			readOffset += elemDesc.CalculateTotalSize();
		}

		return desc;
	}

	blResourceHandleOfType<InlineBufferDescription> InlineBufferDescription::CreateResourceFromBufferDescription(
		std::wstring name,
		const BufferDescription& desc,
		blResourceContainer* container)
	{
		const uint64_t totalSize = InlineBufferDescription::CalculateTotalSize(desc.elementDescriptions);
		blResourceHandleOfType<InlineBufferDescription> handle = 
			container->CreateResourceOfType<InlineBufferDescription>(name, totalSize);
		handle->WriteDescription(desc);
		return handle;
	}

	InlineBufferElementDescription& InlineBufferDescription::Iterator::operator*() const
	{
		return reinterpret_cast<InlineBufferElementDescription&>(*mHead);
	}

	InlineBufferDescription::Iterator& InlineBufferDescription::Iterator::operator++()
	{
		InlineBufferElementDescription& elemDesc = reinterpret_cast<InlineBufferElementDescription&>(*mHead);
		mHead += elemDesc.CalculateTotalSize();
		return *this;
	}

	bool InlineBufferDescription::Iterator::operator!=(const Iterator& other) const
	{
		return mHead != other.mHead;
	}

	InlineBufferDescription::Iterator InlineBufferDescription::Iterator::begin() const
	{
		return Iterator(reinterpret_cast<InlineBufferElementDescription*>(mBegin), mNumElementDescriptions);
	}

	InlineBufferDescription::Iterator InlineBufferDescription::Iterator::end() const
	{
		byte* end = mBegin;

		for (unsigned int i = 0; i < mNumElementDescriptions; ++i)
		{
			InlineBufferElementDescription& elemDesc = reinterpret_cast<InlineBufferElementDescription&>(*end);
			end += elemDesc.CalculateTotalSize();
		}

		return Iterator(reinterpret_cast<InlineBufferElementDescription*>(end), mNumElementDescriptions);
	}


}