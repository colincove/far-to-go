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
#include <blGraphicsCore.h>
#include <minwindef.h>

namespace BoulderLeaf {
	template<typename TResource>
	class blResourceHandleOfType;

	class blResourceContainer;
}

namespace BoulderLeaf::Graphics
{
	enum class BufferFormat
	{
		BoulderLeaf,
		DX12
	};



	enum class BufferElementType : byte
	{
		Float,
		Float2,
		Float3,
		Float4,
		Matrix3x3,
		Matrix4x4,
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
	};

	struct InlineBufferElementDescription
	{
		struct Header
		{
			BufferElementType ElementType;
			uint32_t nameStringLength;
		};

		Header mHeader;

		std::string_view GetName()
		{
			return std::string_view(reinterpret_cast<char*>(this) + CalculateNameStringOffset(), mHeader.nameStringLength * sizeof(char));
		}

		static uint64_t CalculateNameStringOffset()
		{
			return sizeof(InlineBufferElementDescription);
		}

		uint64_t CalculateTotalSize() const
		{
			return sizeof(InlineBufferElementDescription) + mHeader.nameStringLength * sizeof(char);
		}
	};

	struct InlineBufferDescription
	{
		struct Header
		{
			BufferFormat format;
			uint32_t numElementDescriptions;
		};

		Header mHeader;
		
		static uint64_t CalculateBufferElementDescriptionOffset();

		static uint64_t CalculateTotalSize(std::vector<BufferElementDescription> elementDescriptions);

		void WriteDescription(const BufferDescription& desc);

		// IterateElements removed; use Iterator for element traversal

		class Iterator
		{
		private:
			byte* mHead;
			byte* mBegin;
			int mNumElementDescriptions;
		public:
			Iterator(InlineBufferElementDescription* begin, int numElementDescriptions);
			// construct iterator from an InlineBufferDescription reference
			Iterator(InlineBufferDescription& desc);



			InlineBufferElementDescription& operator*() const;

			Iterator& operator++();

			bool operator!=(const Iterator& other) const;

			Iterator begin() const;
			Iterator end() const;
		};

		BufferDescription ToBufferDescription() const;

		static blResourceHandleOfType<InlineBufferDescription> CreateResourceFromBufferDescription(
			std::wstring name,
			const BufferDescription& desc,
			blResourceContainer* container);
	};

	template<typename TElement, BufferFormat TFormat>
	struct BufferDefinitionTemplate
	{
		constexpr static BufferFormat Format = TFormat;
		using ElementType = TElement;
		static const std::vector<BufferElementDescription> Description;
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

	class blDataElementBuffer
	{
	private:
		BufferFormat mFormat;
		std::vector<BufferElementDescription> mElementDescriptions;
		std::unique_ptr<byte[]> mData;
	public:
		blDataElementBuffer(BufferFormat format,
			std::vector<BufferElementDescription> elementDescriptions,
			std::unique_ptr<byte[]> data);

		BufferFormat GetFormat() const { return mFormat; }
		const std::vector<BufferElementDescription>& GetElementDescriptions() const { return mElementDescriptions; }
		const byte* GetData() const { return mData.get(); };
		byte* GetDataMutable() const { return mData.get(); };
	};

	class blDataBufferInterface
	{
	private:
		std::wstring mName;
	public:

		blDataBufferInterface(const std::wstring& name)
			:mName(name)
		{
		}

		blDataBufferInterface() : blDataBufferInterface(L"[BL] Data Buffer") {}

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

		const std::wstring& GetName() const
		{
			return mName;
		}
	};

	template<typename TDataDefinition, typename... Args>
	class blDataBuffer :
		public std::vector<typename TDataDefinition::ElementType>,
		public blDataBufferInterface
	{
	public:
		using BaseVector = std::vector<typename TDataDefinition::ElementType>;
	public:
		using TData = TDataDefinition::ElementType;

		// inherit all constructors from vector
		using BaseVector::vector;

		blDataBuffer(Args&&... args) : blDataBufferInterface(), BaseVector(std::forward<Args>(args)...) {}

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
		UINT mCalculatedStrideLength;
	public:
		blBasicDataBuffer() {}
		blBasicDataBuffer(
			const std::vector<BufferElementDescription>& elementDescriptions,
			size_t count,
			const blBufferElementAdapter& adapter,
			const UINT optEnforcedStride = 0)
			: mElementDescriptions(elementDescriptions),
			mCount(count),
			mCalculatedStrideLength(0)
		{
			mElementSize = GetBufferElementSize(
				elementDescriptions,
				adapter);

			mCalculatedStrideLength = optEnforcedStride == 0 ? 
				static_cast<UINT>(mElementSize) : EnforceByteStrideLength(static_cast<UINT>(mElementSize), optEnforcedStride);

			mBuffer = std::make_unique<byte[]>(mCalculatedStrideLength * count);
		}

		blBasicDataBuffer(
			const blDataBufferInterface& dataBuffer,
			const blBufferElementAdapter& adapter,
			const unsigned int optEnforcedStride = 0)
			: blBasicDataBuffer(
				dataBuffer.GetDataElementDescriptions(),
				dataBuffer.Count(),
				adapter,
				optEnforcedStride)
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
			return mBuffer[index * mCalculatedStrideLength];
		};

		virtual byte& GetMutable(size_t index)
		{
			return mBuffer[index * mCalculatedStrideLength];
		};

		virtual size_t GetElementSize()
		{
			return mCalculatedStrideLength;
		}
	};
}

BL_RESOURCE(blDataBufferInterface, eResourceType::DataBuffer);
BL_RESOURCE(blDataElementBuffer, eResourceType::DataBuffer);

namespace BoulderLeaf::Graphics
{
	class blVertexBufferWithPassBuffer
	{
	private:
		blDataElementBufferResourcePtr mPassElementBuffer;
		blDataBufferInterfaceResourcePtr mVertexBuffer;
	public:
		blVertexBufferWithPassBuffer(
			blDataElementBufferResourcePtr passElementBuffer,
			blDataBufferInterfaceResourcePtr vertexBuffer);

		const blDataElementBufferResourcePtr GetPassElementBuffer() const { return mPassElementBuffer; }
		const blDataBufferInterfaceResourcePtr GetVertexBuffer() const { return mVertexBuffer; }
	};
}

BL_RESOURCE(blVertexBufferWithPassBuffer, eResourceType::DataBuffer)