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

namespace BoulderLeaf::Graphics
{
	template<typename tFrom, typename tTo>
	tTo From(const tFrom& from);

	class blMeshStorage
	{
		template<typename TVertex>
		friend class blMesh;

		struct Header
		{
			Header(
				size_t vertexCount,
				size_t vertexSize,
				size_t indexCount)
				: mVertexCount(vertexCount),
				mVertexSize(vertexSize),
				mIndexCount(indexCount)
			{

			}

			size_t mVertexCount;
			size_t mVertexSize;
			size_t mIndexCount;

			size_t GetVertexBufferSize() const
			{
				return mVertexCount * mVertexSize;
			}

			size_t GetIndexBufferSize() const
			{
				return mIndexCount * sizeof(size_t);
			}
		};

		friend bool operator==(const Header& lhs, const Header& rhs)
		{
			return lhs.mVertexCount == rhs.mVertexCount &&
				lhs.mVertexSize == rhs.mVertexSize &&
				lhs.mIndexCount == rhs.mIndexCount;
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
			const size_t* indices,
			const size_t indexCount,
			const size_t vertexSize);

		blMeshStorage(const blMeshStorage& other);
		blMeshStorage(blMeshStorage&& other) noexcept;

	private:
		std::unique_ptr<byte[]> mBuffer;
		byte* mVertexDataStart;
		size_t* mIndexDataStart;
	private:
		static size_t GetBufferSize(const Header& header);
	public:
		template<typename TVertexFrom, typename TVertexTo>
		static blMeshStorage To(const blMeshStorage& from)
		{
			blMeshStorage result(from.GetVertexCount(), from.GetIndexCount(), sizeof(TVertexTo));
			memcpy(static_cast<void*>(result.mBuffer.get()), static_cast<const void*>(from.mBuffer.get()), sizeof(Header) + from.GetIndexCount());

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
		
		const size_t& GetIndex(size_t index) const;
		size_t& GetIndexMutable(size_t index);
		
		const byte* begin() const;
		const byte* end() const;

		bool IsValid() const;
	};

	template<typename TVertex>
	class blMesh
	{
	private:
		blMeshStorage mStorage;
	public:
		blMesh() : mStorage() {}
		blMesh(blMeshStorage storage) : mStorage(storage){}
		blMesh(size_t vertexCount) : blMesh<TVertex>::blMesh(vertexCount, 0) {}
		blMesh(size_t vertexCount, size_t indexCount)
			:mStorage(
				vertexCount,
				indexCount,
				sizeof(TVertex)){}

		blMesh(
			TVertex vertices[]) : mStorage(static_cast<byte[]>(vertices), sizeof(TVertex))
		{
		}

		blMesh(
			const TVertex* vertices,
			const size_t vertexCount,
			const size_t* indices,
			const size_t indexCount) : mStorage(reinterpret_cast<const byte*>(vertices), vertexCount, indices, indexCount, sizeof(TVertex))
		{

		}

		blMesh(const blMesh<TVertex>& other):mStorage(other.mStorage){}
		blMesh(blMesh<TVertex>&& other) noexcept :mStorage(std::move(other.mStorage)) {}
	public:
		const size_t& GetVertexCount() const { return mStorage.GetVertexCount(); }
		const size_t& GetVertexSize() const { return mStorage.GetVertexSize(); }
		const size_t& GetIndexCount() const { return mStorage.GetIndexCount(); }

		const TVertex& GetVertex(size_t index) const { return *reinterpret_cast<const TVertex*>(mStorage.GetVertex(index)); }
		TVertex& GetVertexMutable(size_t index) { return *reinterpret_cast<TVertex*>(mStorage.GetVertexMutable(index)); }

		const size_t& GetIndex(size_t index) const { return mStorage.GetIndex(index); }
		size_t& GetIndexMutable(size_t index) { return mStorage.GetIndexMutable(index); }

		bool IsValid() const { return mStorage.IsValid(); }

		const blMeshStorage& GetStorage() const { return mStorage; }
		blMeshStorage& GetStorageMutable() { return mStorage; }
	};

	struct StandardVertex
	{
		Math::Vector3 Position;
		Math::Vector3 Normal;
		Math::Vector4 Colour;
		Math::Vector2 UV;
	};

	inline bool operator==(const StandardVertex& lhs, const StandardVertex& rhs)
	{
		return lhs.Colour == rhs.Colour && lhs.Normal == rhs.Normal && lhs.Position == rhs.Position && lhs.UV == rhs.UV;
	}

	inline bool operator!=(const StandardVertex& lhs, const StandardVertex& rhs) { return !(lhs == rhs); }

	using StandardMesh = blMesh<StandardVertex>;

	StandardMesh BuildConeMesh();

	template<typename tFrom, typename tTo, typename tFromVertex, typename tToVertex>
	tTo CastMeshFrom(const tFrom& from, std::function<tToVertex(tFromVertex)> fromVertexFunction)
	{
		const size_t size = from.mVertices.size();
		tTo result(size);

		for (size_t i = 0; i < size; ++i)
		{
			//result.mVertices[i] = From<tFromVertex, tToVertex>(from.mVertices[i]);
			//result.mVertices[i] = fromVertexFunction(
		}

		return std::move(result);
	}
}