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

	blBufferElementDescriptionResource::blBufferElementDescriptionResource(
		blResourceStream& stream, 
		const BufferElementDescription& element)
		: blBaseResource(stream), 
		mElementType(element.ElementType), 
		mNameStringResource(stream, element.Name)
	{
		
	}

	const std::string_view blBufferElementDescriptionResource::GetName() const
	{
		return mNameStringResource.GetStringView();
	}

	blBufferDescriptionResource::blBufferDescriptionResource(
		blResourceStream& stream,
		const BufferDescription& desc)
		: mFormat(desc.format), blListDynamicResource(stream, desc.elementDescriptions)
	{

	}

	BufferDescription blBufferDescriptionResource::GetBufferDescription() const
	{
		BufferDescription desc;
		desc.format = mFormat;
		for (const blBufferElementDescriptionResource& elementResource : blBufferDescriptionResource::ConstIterator(this))
		{
			desc.elementDescriptions.push_back({ std::string(elementResource.GetName()), elementResource.mElementType });
		}
		return desc;
	}

	blArrayBufferResource::blArrayBufferResource(
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		blResourceRefOfType<blListResource> bufferResourceRef) 
		: blBaseResource(),
		mDescriptionResourceRef(descriptionResourceRef),
		mBufferResourceRef(bufferResourceRef)
	{

	}

	blResourceHandleOfType<blArrayBufferResource> CreateArrayBufferResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		const void* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount)
	{
		blResourceHandleOfType<blListResource> arrayData = resourceContainer->CreateResourceOfTypeWithDynamicSize<blListResource>(
			name + L"Data",
			vertexCount,
			vertexSize,
			vertices
		);

		return resourceContainer->CreateResourceOfType<blArrayBufferResource>(
			name + L"Array",
			descriptionResourceRef,
			arrayData.GetRef()
		);
	}

	blResourceHandleOfType<blArrayBufferResource> CreateArrayBufferResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		const blArrayBufferResource& sourceArray,
		const blBufferElementAdapter& adapter)
	{
		blResourceHandleOfType<blListResource> sourceListResourceHandle = blResourceHandleOfType<blListResource>(
			resourceContainer->CreateHandleFromRefOfType<blListResource>(sourceArray.mBufferResourceRef));

		blResourceHandleOfType<blBufferDescriptionResource> sourceDescriptionResourceHandle = blResourceHandleOfType<blBufferDescriptionResource>(
			resourceContainer->CreateHandleFromRefOfType<blBufferDescriptionResource>(sourceArray.mDescriptionResourceRef));

		const BufferDescription sourceDescription = sourceDescriptionResourceHandle->GetBufferDescription();

		blResourceHandleOfType<blListResource> arrayData = resourceContainer->CreateResourceOfTypeWithDynamicSize<blListResource>(
			name + L"Data",
			sourceListResourceHandle->mCount,
			adapter.SizeOf(sourceDescription)
		);

		MarshalBuffer(
			sourceListResourceHandle->mCount,
			sourceDescription.elementDescriptions,
			[&sourceListResourceHandle, &sourceDescription](size_t index) -> const byte*
			{
				return &sourceListResourceHandle->Get<byte>((uint32_t) index);
			},
			[&arrayData](size_t index) -> byte*
			{
				return &arrayData->GetMutable<byte>((uint32_t) index);
			},
			adapter
		);

		return resourceContainer->CreateResourceOfType<blArrayBufferResource>(
			name + L"Array",
			sourceDescriptionResourceHandle.GetRef(),
			arrayData.GetRef()
		);
	}
}