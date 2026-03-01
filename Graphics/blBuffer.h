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
#include <blTypes.h>
#include <functional>
#include <Resources/blResource.h>
#include <stdexcept>
#include <string>

namespace BoulderLeaf::Graphics
{
	enum class BufferElementType : byte
	{
		Float2,
		Float3,
		Float4,
		Matrix3x3,
		Matrix4x4
	};

	struct BufferElementDescription
	{
		std::string Name;
		BufferElementType ElementType;
	};

	template<typename TElement>
	struct BufferDefinition
	{
		using ElementType = TElement;
		static const std::vector<BufferElementDescription> Description;
	};

	class blBufferElementAdapter
	{
	public:
		virtual size_t SizeOfElement(BufferElementType Type) const = 0;
		virtual void MarshalVector2(const Math::Vector2& srcElement, byte* destElement) const = 0;
		virtual void MarshalVector3(const Math::Vector3& srcElement, byte* destElement) const = 0;
		virtual void MarshalVector4(const Math::Vector4& srcElement, byte* destElement) const = 0;
		virtual void MarshalMatrix3x3(const Math::Matrix3x3& srcElement, byte* destElement) const = 0;
		virtual void MarshalMatrix4x4(const Math::Matrix4x4& srcElement, byte* destElement) const = 0;
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

	class blDataBufferInterface
	{
	public:
		virtual const std::vector<BufferElementDescription>& GetDataElementDescriptions() const
		{
			static std::vector<BufferElementDescription> defaultDescriptions;
			//dumb. no need for this. just nonsense complexity with resources
			return defaultDescriptions;
		}

	public:
		virtual size_t Count() const 
		{ 
			throw std::invalid_argument("Not Implemented. Abstract.");
		};

		virtual const byte& Get(size_t index) const 
		{
			throw std::invalid_argument("Not Implemented. Abstract.");
		};

		virtual byte& GetMutable(size_t index) 
		{
			throw std::invalid_argument("Not Implemented. Abstract.");
		};

		virtual size_t GetElementSize()
		{
			throw std::invalid_argument("Not Implemented. Abstract.");
		}
	};

	template<typename TDataDefinition>
	class blDataBuffer :
		public std::vector<typename TDataDefinition::ElementType>,
		public blDataBufferInterface
	{
	private:
		using BaseVector = std::vector<typename TDataDefinition::ElementType>;
	public:
		using TData = TDataDefinition::ElementType;

		// inherit all constructors from vector
		using BaseVector::vector;

		virtual size_t Count() const override { return BaseVector::size(); };
		virtual const byte& Get(size_t index) const override
		{
			return reinterpret_cast<const byte&>((*this)[index]);
		}

		virtual size_t GetElementSize()
		{
			return sizeof(TData);
		}

		virtual byte& GetMutable(size_t index) override
		{
			return reinterpret_cast<byte&>((*this)[index]);
		}

		virtual const std::vector<BufferElementDescription>& GetDataElementDescriptions() const override
		{
			return TDataDefinition::Description;
		}
	};

	class blBasicDataBuffer : public blDataBufferInterface
	{
	private:
		std::unique_ptr<byte[]> mBuffer;
		std::vector<BufferElementDescription> mElementDescriptions;
		size_t mElementSize;
		size_t mCount;
	public:
		blBasicDataBuffer() {}
		blBasicDataBuffer(
			const std::vector<BufferElementDescription>& elementDescriptions, 
			size_t count,
			const blBufferElementAdapter& adapter)
			: mElementDescriptions(elementDescriptions),
			mCount(count)
		{
			mElementSize = GetBufferElementSize(
				elementDescriptions,
				adapter);

			mBuffer = std::make_unique<byte[]>(mElementSize * count);
		}

		blBasicDataBuffer(
			const blDataBufferInterface& dataBuffer,
			const blBufferElementAdapter& adapter)
			: blBasicDataBuffer(
				dataBuffer.GetDataElementDescriptions(),
				dataBuffer.Count(),
				adapter)
		{
			MarshalBuffer(
				dataBuffer.Count(),
				dataBuffer.GetDataElementDescriptions(),
				[&dataBuffer](size_t index)
				{
					return &dataBuffer.Get(index);
				},
				[this](size_t index)
				{
					return &this->GetMutable(index);
				},
				adapter);
		}

		virtual const std::vector<BufferElementDescription>& GetDataElementDescriptions() const override
		{
			return mElementDescriptions;
		}

		virtual size_t Count() const
		{
			return mCount;
		};

		virtual const byte& Get(size_t index) const
		{
			return mBuffer[index * mElementSize];
		};

		virtual byte& GetMutable(size_t index)
		{
			return mBuffer[index * mElementSize];
		};

		virtual size_t GetElementSize()
		{
			return mElementSize;
		}
	};
}

BL_RESOURCE(blDataBufferInterface, eResourceType::DataBuffer);