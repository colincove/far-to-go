#pragma once

#include <blBuffer.h>
#include <blVertex.h>

namespace BoulderLeaf::Graphics
{
	template<typename tFrom, typename tTo>
	tTo From(const tFrom& from);

	class blMeshStorage
	{
		template<typename TVertex>
		friend class blMesh;
		friend class blMeshBase;
	public:
		using index = std::uint16_t;

		enum Version : int
		{
			Initial,
		};

		struct Header
		{
			Header() : Header(0, 0, 0, BufferFormat::BoulderLeaf) {}
			Header(
				size_t vertexCount,
				size_t indexCount,
				size_t vertexSize,
				BufferFormat format)
				: mVersion(Version::Initial),
				mVertexCount(vertexCount),
				mIndexCount(indexCount),
				mVertexSize(vertexSize),
				mFormat(format)
			{

			}

			BufferFormat mFormat = BufferFormat::BoulderLeaf;
			Version mVersion = Version::Initial;
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
			return lhs.mVersion == rhs.mVersion &&
				lhs.mVertexCount == rhs.mVertexCount &&
				lhs.mIndexCount == rhs.mIndexCount &&
				lhs.mVertexSize == rhs.mVertexSize;
		};

	public:
		blMeshStorage();
		blMeshStorage(
			size_t vertexCount,
			size_t indexCount,
			size_t vertexSize,
			BufferFormat format);

		blMeshStorage(
			byte vertices[],
			size_t vertexSize,
			BufferFormat format);

		blMeshStorage(
			const byte* vertices,
			const size_t vertexCount,
			const index* indices,
			const size_t indexCount,
			const size_t vertexSize,
			BufferFormat format);

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
			blMeshStorage result(from.GetVertexCount(), from.GetIndexCount(), sizeof(TVertexTo), from.GetFormat());
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
		const BufferFormat& GetFormat() const;

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

		byte* VertexBeginMutable()
		{
			return mVertexDataStart;
		}

		const index* IndexBegin() const
		{
			return mIndexDataStart;
		}

		index* IndexBeginMutable()
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

		void GraftMeshData(const blMeshStorage& other, const size_t index, const size_t vertex, bool offsetVertexIndices);
	};

	class blMeshBase
	{
	protected:
		blMeshStorage mStorage;
	public:
		blMeshBase() : mStorage() {}
		blMeshBase(const blMeshStorage& storage) : mStorage(storage) {}
		blMeshBase(blMeshStorage&& storage) : mStorage(std::move(storage)) {}
		blMeshBase(size_t vertexCount, size_t indexCount, const size_t vertexSize, BufferFormat format)
			:mStorage(
				vertexCount,
				indexCount,
				vertexSize,
				format) {
		}

		blMeshBase(
			byte vertices[],
			const size_t vertexSize,
			BufferFormat format) : mStorage(vertices, vertexSize, format)
		{
		}

		blMeshBase(
			const byte* vertices,
			const size_t vertexCount,
			const blMeshStorage::index* indices,
			const size_t indexCount,
			const size_t vertexSize,
			BufferFormat format) : mStorage(vertices, vertexCount, indices, indexCount, vertexSize, format)
		{

		}

		blMeshBase(const blMeshBase& other) :mStorage(other.mStorage) {}
		blMeshBase(blMeshBase&& other) noexcept :mStorage(std::move(other.mStorage)) {}

		virtual const std::vector<VertexElementDescription>& GetVertexElementDescriptions() const
		{
			return VertexElementDescriptions;
		};
	public:
		bool IsValid() const { return mStorage.IsValid(); }
		const blMeshStorage& GetStorage() const { return mStorage; }
		blMeshStorage& GetStorageMutable() { return mStorage; }
	public:
		const size_t& GetVertexCount() const { return mStorage.GetVertexCount(); }
		const size_t& GetVertexSize() const { return mStorage.GetVertexSize(); }
		const size_t& GetIndexCount() const { return mStorage.GetIndexCount(); }
		const BufferFormat& GetFormat() const { return mStorage.GetFormat(); }

		const blMeshStorage::index& GetIndex(size_t index) const { return mStorage.GetIndex(index); }
		blMeshStorage::index& GetIndexMutable(size_t index) { return mStorage.GetIndexMutable(index); }

		std::vector<VertexElementDescription> VertexElementDescriptions;
	};

	template<typename TVertexDefinition>
	class blMesh : public blMeshBase
	{
	public:
		using TVertexDef = TVertexDefinition;
		using TVertex = TVertexDefinition::VertexType;
		constexpr static BufferFormat Format = TVertexDef::Format;

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

		static uint64_t CalculateSizeOfPrototype(
			const Prototype& prototype,
			const BufferDescription& bufferDescription,
			const blBufferElementAdapter& adapter)
		{
			blMeshStorage::Header header = blMeshStorage::Header(
				prototype.vertices.size(),
				prototype.indices.size(),
				adapter.SizeOf(bufferDescription),
				adapter.GetFormat()
			);

			return sizeof(blMeshStorage::Header) + blMeshStorage::GetBufferSize(header);
		}

	public:
		blMesh() : blMeshBase() {}
		blMesh(const blMeshStorage& storage) : blMeshBase(storage) {}
		blMesh(size_t vertexCount) : blMesh<TVertexDefinition>::blMesh(vertexCount, 0) {}
		blMesh(size_t vertexCount, size_t indexCount)
			:blMeshBase(
				vertexCount,
				indexCount,
				sizeof(TVertex),
				Format) {
		}

		blMesh(
			TVertex vertices[]) : blMeshBase(static_cast<byte[]>(vertices), sizeof(TVertex), Format)
		{
		}

		blMesh(
			const TVertex* vertices,
			const size_t vertexCount,
			const blMeshStorage::index* indices,
			const size_t indexCount) : blMeshBase(reinterpret_cast<const byte*>(vertices), vertexCount, indices, indexCount, sizeof(TVertex), Format)
		{

		}

		blMesh(const Prototype& prototype) : blMesh(
			prototype.vertices.data(),
			prototype.vertices.size(),
			prototype.indices.data(),
			prototype.indices.size())
		{

		}

		blMesh(const blMesh<TVertexDefinition>& other) :blMeshBase(other.mStorage) {}
		blMesh(blMesh<TVertexDefinition>&& other) noexcept :blMeshBase(std::move(other.mStorage)) {}

		virtual const std::vector<VertexElementDescription>& GetVertexElementDescriptions() const override
		{
			return TVertexDefinition::Description;
		}

	public:
		const TVertex& GetVertex(size_t index) const { return *reinterpret_cast<const TVertex*>(mStorage.GetVertex(index)); }
		TVertex& GetVertexMutable(size_t index) { return *reinterpret_cast<TVertex*>(mStorage.GetVertexMutable(index)); }
	};

	using StandardMesh = blMesh<StandardVertexDefinition>;

	template<typename TVertexDefinition>
	void SubdividePrototype(typename blMesh<TVertexDefinition>::Prototype& prototype)
	{
		struct Triangle
		{
			union
			{
				struct
				{
					blMeshStorage::index i0;
					blMeshStorage::index i1;
					blMeshStorage::index i2;
				};

				blMeshStorage::index data[3];
			};
		};

		const size_t indexCount = prototype.indices.size();

		//we will be adding 9 new indices for every existing triangle. 
		//reserve the space up-front, so that we do not resize during operation.
		prototype.indices.reserve(indexCount + (indexCount / 3) * 9);

		for (size_t i = 0; (i + 2) < indexCount; i += 3)
		{
			Triangle& triangleToSubdivide = *reinterpret_cast<Triangle*>(prototype.indices.data() + i);

			const std::array<Math::Vector3, 3> newPositions = {
				prototype.vertices[triangleToSubdivide.i0].Position + (prototype.vertices[triangleToSubdivide.i1].Position - prototype.vertices[triangleToSubdivide.i0].Position) * 0.5f,
				prototype.vertices[triangleToSubdivide.i1].Position + (prototype.vertices[triangleToSubdivide.i2].Position - prototype.vertices[triangleToSubdivide.i1].Position) * 0.5f,
				prototype.vertices[triangleToSubdivide.i2].Position + (prototype.vertices[triangleToSubdivide.i0].Position - prototype.vertices[triangleToSubdivide.i2].Position) * 0.5f
			};

			// Add the new vertices (copies of the existing ones with adjusted positions)
			for (int np = 0; np < 3; ++np)
			{
				prototype.vertices.push_back(prototype.vertices[triangleToSubdivide.data[np]]);
				prototype.vertices.back().Position = newPositions[np];
			}

			const size_t newVertexSize = prototype.vertices.size();
			std::array<blMeshStorage::index, 3> newIndices =
			{
				static_cast<blMeshStorage::index>(newVertexSize - 3),
				static_cast<blMeshStorage::index>(newVertexSize - 2),
				static_cast<blMeshStorage::index>(newVertexSize - 1)
			};

			// Append new triangles (indices) to the indices vector
			prototype.indices.push_back(newIndices[0]);
			prototype.indices.push_back(triangleToSubdivide.i1);
			prototype.indices.push_back(newIndices[1]);

			prototype.indices.push_back(newIndices[1]);
			prototype.indices.push_back(triangleToSubdivide.i2);
			prototype.indices.push_back(newIndices[2]);

			prototype.indices.push_back(newIndices[2]);
			prototype.indices.push_back(newIndices[0]);
			prototype.indices.push_back(newIndices[1]);

			// Update the original triangle to reference the new vertices
			triangleToSubdivide.i1 = newIndices[0];
			triangleToSubdivide.i2 = newIndices[2];
		}
	};
}

namespace BoulderLeaf::Graphics
{
	const StandardMesh& Cube();

	StandardMesh CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount);
	StandardMesh CreateGeosphere(float radius, unsigned int numSubdivisions);

}