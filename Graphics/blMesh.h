#pragma once

#include <blVector3.inl>
#include <blVector4.inl>
#include <blVector2.inl>
#include <vector>
#include <functional>
#include <memory>
#include <blTypes.h>
#include <assert.h>
#include <blCore.h>
#include <Resources/blResource.h>
#include <string>
#include <blBuffer.h>

namespace BoulderLeaf::Graphics
{
	template<typename tFrom, typename tTo>
	tTo From(const tFrom& from);
	using VertexElementType = BufferElementType;
	using VertexElementDescription = BufferElementDescription;

	template<typename TVertex>
	struct VertexDefinition : public BufferDefinition<TVertex>
	{
		using VertexType = TVertex;
	};

	struct StandardVertex
	{
		Math::Vector3 Position;
		Math::Vector3 Normal;
		Math::Vector3 Tangent;
		Math::Vector4 Colour;
		Math::Vector2 UV;
	};

	class blMeshStorage
	{
		template<typename TVertex>
		friend class blMesh;
		friend class blMeshBase;
	public:
		using index = std::uint16_t;

		struct Header
		{
			Header(
				size_t vertexCount,
				size_t indexCount,
				size_t vertexSize)
				: mVertexCount(vertexCount),
				mIndexCount(indexCount),
				mVertexSize(vertexSize)
			{

			}

			size_t mVertexCount;
			size_t mVertexSize;
			size_t mIndexCount;

			size_t GetIndexBufferSize() const
			{
				return mIndexCount * sizeof(index);
			}

			size_t GetVertexBufferSize() const
			{
				return mVertexCount * mVertexSize;
			}
		};

		friend bool operator==(const Header& lhs, const Header& rhs)
		{
			return lhs.mVertexCount == rhs.mVertexCount &&
				lhs.mIndexCount == rhs.mIndexCount &&
				lhs.mVertexSize == rhs.mVertexSize;
		};

	public:
		blMeshStorage();
		blMeshStorage(
			size_t vertexCount,
			size_t indexCount,
			size_t vertexSize);

		blMeshStorage(
			byte vertices[],
			size_t vertexSize);

		blMeshStorage(
			const byte* vertices,
			const size_t vertexCount,
			const index* indices,
			const size_t indexCount,
			const size_t vertexSize);

		blMeshStorage(const blMeshStorage& other);
		blMeshStorage(blMeshStorage&& other) noexcept;

	private:
		std::unique_ptr<byte[]> mBuffer;
		byte* mVertexDataStart;
		index* mIndexDataStart;
	private:
		static size_t GetBufferSize(const Header& header);
	public:
		static std::unique_ptr<byte[]> CreateVertexBuffer(std::vector<const blMeshStorage*> storageArray);
		static std::unique_ptr<index[]> CreateIndexBuffer(std::vector<const blMeshStorage*> storageArray);
	public:
		template<typename TVertexFrom, typename TVertexTo>
		static blMeshStorage To(const blMeshStorage& from)
		{
			blMeshStorage result(from.GetVertexCount(), from.GetIndexCount(), sizeof(TVertexTo));
			memcpy(static_cast<void*>(result.mIndexDataStart), static_cast<const void*>(from.mIndexDataStart), from.GetIndexCount() * sizeof(index));
			
			for (int i = 0; i < from.GetVertexCount(); ++i)
			{
				TVertexTo& vertex = *reinterpret_cast<TVertexTo*>(result.GetVertexMutable(i));
				vertex = From<TVertexFrom, TVertexTo>(*reinterpret_cast<const TVertexFrom*>(from.GetVertex(i)));
			}

			return result;
		};
	private:
		Header& GetHeaderMutable();
		void InitializeIndexingPointers();
	public:
		const Header& GetHeader() const;
		const size_t& GetVertexCount() const;
		const size_t& GetVertexSize() const;
		const size_t& GetIndexCount() const;

		const void* GetVertex(size_t index) const;
		void* GetVertexMutable(size_t index);
		
		const index& GetIndex(size_t index) const;
		index& GetIndexMutable(size_t index);
		
		const byte* begin() const;
		const byte* end() const;

		const byte* VertexBegin() const
		{
			return mVertexDataStart;
		}

		const index* IndexBegin() const
		{
			return mIndexDataStart;
		}

		bool IsValid() const;

		inline void operator=(blMeshStorage&& rhs)
		{
			mBuffer = std::move(rhs.mBuffer);
			rhs.mIndexDataStart = nullptr;
			rhs.mVertexDataStart = nullptr;
			InitializeIndexingPointers();
		}
	};

	class blMeshBase
	{
	protected:
		blMeshStorage mStorage;
	public:
		blMeshBase() : mStorage() {}
		blMeshBase(const blMeshStorage& storage) : mStorage(storage) {}
		blMeshBase(blMeshStorage&& storage) : mStorage(std::move(storage)) {}
		blMeshBase(size_t vertexCount, size_t indexCount, const size_t vertexSize)
			:mStorage(
				vertexCount,
				indexCount,
				vertexSize) {
		}

		blMeshBase(
			byte vertices[],
			const size_t vertexSize) : mStorage(vertices, vertexSize)
		{
		}

		blMeshBase(
			const byte* vertices,
			const size_t vertexCount,
			const blMeshStorage::index* indices,
			const size_t indexCount,
			const size_t vertexSize) : mStorage(vertices, vertexCount, indices, indexCount, vertexSize)
		{

		}

		blMeshBase(const blMeshBase& other) :mStorage(other.mStorage) {}
		blMeshBase(blMeshBase&& other) noexcept :mStorage(std::move(other.mStorage)) {}

		virtual const std::vector<VertexElementDescription>& GetVertexElementDescriptions() const 
		{
			static std::vector<VertexElementDescription> defaultDescriptions;
			//dumb. no need for this. just nonsense complexity with resources
			return defaultDescriptions;
		};
	public:
		bool IsValid() const { return mStorage.IsValid(); }
		const blMeshStorage& GetStorage() const { return mStorage; }
		blMeshStorage& GetStorageMutable() { return mStorage; }
	public:
		const size_t& GetVertexCount() const { return mStorage.GetVertexCount(); }
		const size_t& GetVertexSize() const { return mStorage.GetVertexSize(); }
		const size_t& GetIndexCount() const { return mStorage.GetIndexCount(); }

		const blMeshStorage::index& GetIndex(size_t index) const { return mStorage.GetIndex(index); }
		blMeshStorage::index& GetIndexMutable(size_t index) { return mStorage.GetIndexMutable(index); }
	};

	template<typename TVertexDefinition>
	class blMesh : public blMeshBase
	{
	public:
		using TVertex = TVertexDefinition::VertexType;
		struct Prototype
		{
			std::vector<TVertex> vertices;
			std::vector<blMeshStorage::index> indices;
		};

		using TPrototype = Prototype;

		static Prototype& GetPrototype()
		{
			static Prototype prototype;
			prototype.indices.clear();
			prototype.vertices.clear();
			return prototype;
		}
	public:
		blMesh() : blMeshBase() {}
		blMesh(const blMeshStorage& storage) : blMeshBase(storage){}
		blMesh(size_t vertexCount) : blMesh<TVertexDefinition>::blMesh(vertexCount, 0) {}
		blMesh(size_t vertexCount, size_t indexCount)
			:blMeshBase(
				vertexCount,
				indexCount,
				sizeof(TVertex)){}

		blMesh(
			TVertex vertices[]) : blMeshBase(static_cast<byte[]>(vertices), sizeof(TVertex))
		{
		}

		blMesh(
			const TVertex* vertices,
			const size_t vertexCount,
			const blMeshStorage::index* indices,
			const size_t indexCount) : blMeshBase(reinterpret_cast<const byte*>(vertices), vertexCount, indices, indexCount, sizeof(TVertex))
		{

		}

		blMesh(const Prototype& prototype) : blMesh(
			prototype.vertices.data(), 
			prototype.vertices.size(),
			prototype.indices.data(),
			prototype.indices.size())
		{

		}

		blMesh(const blMesh<TVertexDefinition>& other):blMeshBase(other.mStorage){}
		blMesh(blMesh<TVertexDefinition>&& other) noexcept :blMeshBase(std::move(other.mStorage)) {}

		virtual const std::vector<VertexElementDescription>& GetVertexElementDescriptions() const override
		{
			return TVertexDefinition::Description;
		}

	public:
		const TVertex& GetVertex(size_t index) const { return *reinterpret_cast<const TVertex*>(mStorage.GetVertex(index)); }
		TVertex& GetVertexMutable(size_t index) { return *reinterpret_cast<TVertex*>(mStorage.GetVertexMutable(index)); }
	};

	using StandardVertexDefinition = VertexDefinition<StandardVertex>;

	inline bool operator==(const StandardVertex& lhs, const StandardVertex& rhs)
	{
		return lhs.Colour == rhs.Colour && lhs.Normal == rhs.Normal && lhs.Position == rhs.Position && lhs.UV == rhs.UV;
	}

	inline bool operator!=(const StandardVertex& lhs, const StandardVertex& rhs) { return !(lhs == rhs); }

	using StandardMesh = blMesh<StandardVertexDefinition>;
	using blMeshBaseUniquePtr = std::unique_ptr<::BoulderLeaf::Graphics::blMeshBase>;
	using StandardMeshUniquePtr = std::unique_ptr<::BoulderLeaf::Graphics::StandardMesh>;
}

BL_RESOURCE(blMeshBase, eResourceType::Mesh);
BL_RESOURCE(StandardMesh, eResourceType::Mesh);