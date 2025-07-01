#include <blMesh.h>

namespace BoulderLeaf::Graphics
{
	size_t blMeshStorage::GetBufferSize(const Header& header)
	{
		return sizeof(Header) +
			header.mVertexCount * header.mVertexSize +
			header.mIndexCount * sizeof(blMeshStorage::index);
	}

	blMeshStorage::blMeshStorage() : 
		mBuffer(nullptr), 
		mVertexDataStart(nullptr), 
		mIndexDataStart(nullptr)
	{

	}

	blMeshStorage::blMeshStorage(
		size_t vertexCount,
		size_t indexCount,
		size_t vertexSize) : blMeshStorage()
	{
		const Header header = Header(vertexCount, vertexSize, indexCount);
		const size_t bufferSize = GetBufferSize(header);
		mBuffer = std::make_unique<byte[]>(bufferSize);
		GetHeaderMutable() = header;
		InitializeIndexingPointers();
	}

	blMeshStorage::blMeshStorage(
		byte vertices[],
		size_t vertexSize) : blMeshStorage((size_t)(sizeof(vertices) / vertexSize), 0, vertexSize)
	{
		memcpy(mVertexDataStart, vertices, sizeof(vertices));
	}

	blMeshStorage::blMeshStorage(
		const byte* vertices,
		const size_t vertexCount,
		const index* indices,
		const size_t indexCount,
		const size_t vertexSize) : blMeshStorage(
			vertexCount,
			indexCount,
			vertexSize)
	{
		memcpy(mVertexDataStart, vertices, vertexCount * vertexSize);
		memcpy(mIndexDataStart, indices, indexCount * sizeof(index));
	}

	blMeshStorage::blMeshStorage(blMeshStorage&& other) noexcept
	{
		mBuffer = std::move(other.mBuffer);
		other.mIndexDataStart = nullptr;
		other.mVertexDataStart = nullptr;
		InitializeIndexingPointers();
	}

	blMeshStorage::blMeshStorage(const blMeshStorage& other)
	{
		const size_t bufferSize = GetBufferSize(other.GetHeader());
		mBuffer = std::make_unique<byte[]>(bufferSize);
		memcpy(static_cast<void*>(mBuffer.get()), other.begin(), bufferSize);
		InitializeIndexingPointers();
	}

	void blMeshStorage::InitializeIndexingPointers()
	{
		const Header& header = GetHeader();
		mIndexDataStart = reinterpret_cast<blMeshStorage::index*>(mBuffer.get() + sizeof(Header));
		mVertexDataStart = reinterpret_cast<byte*>(mIndexDataStart + header.mIndexCount);
	}

	const blMeshStorage::Header& blMeshStorage::GetHeader() const
	{
		return *reinterpret_cast<const blMeshStorage::Header*>(mBuffer.get());
	}

	blMeshStorage::Header& blMeshStorage::GetHeaderMutable()
	{
		return *reinterpret_cast<blMeshStorage::Header*>(mBuffer.get());
	}

	const size_t& blMeshStorage::GetVertexCount() const
	{
		return GetHeader().mVertexCount;
	}

	const size_t& blMeshStorage::GetVertexSize() const
	{
		return GetHeader().mVertexSize;
	}

	const size_t& blMeshStorage::GetIndexCount() const
	{
		return GetHeader().mIndexCount;
	}

	const void* blMeshStorage::GetVertex(size_t index) const
	{
		return mVertexDataStart + index * GetHeader().mVertexSize;
	}

	void* blMeshStorage::GetVertexMutable(size_t index)
	{
		return mVertexDataStart + index * GetHeader().mVertexSize;
	}

	const blMeshStorage::index& blMeshStorage::GetIndex(size_t index) const
	{
		return *(mIndexDataStart + index);
	}

	blMeshStorage::index& blMeshStorage::GetIndexMutable(size_t index)
	{
		return *(mIndexDataStart + index);
	}

	const byte* blMeshStorage::begin() const
	{
		return mBuffer.get();
	}

	const byte* blMeshStorage::end() const
	{
		return begin() + sizeof(byte) * GetBufferSize(GetHeader());
	}

	bool blMeshStorage::IsValid() const { return mBuffer != nullptr; }
}