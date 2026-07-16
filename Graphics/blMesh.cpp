#include <blMesh.h>
#include <blVector3.inl>
#include <array>

namespace BoulderLeaf::Graphics
{
	size_t blMeshStorage::GetBufferSize(const Header& header)
	{
		return sizeof(Header) +
			header.mVertexCount * header.mVertexSize +
			header.mIndexCount * sizeof(blMeshStorage::index);
	}

	std::unique_ptr<byte[]> blMeshStorage::CreateVertexBuffer(std::vector<const blMeshStorage*> storageArray)
	{
		size_t totalBufferSize = 0;

		for (const auto& storage : storageArray)
		{
			totalBufferSize += storage->GetHeader().GetVertexBufferSize();
		}

		std::unique_ptr<byte[]> buffer = std::make_unique<byte[]>(totalBufferSize);

		size_t offset = 0;
		for (size_t i = 0; i < storageArray.size(); ++i)
		{
			const blMeshStorage* storage = storageArray[i];
			const size_t vertexBufferSize = storage->GetHeader().GetVertexBufferSize();
			memcpy(buffer.get() + offset, storage->mVertexDataStart, vertexBufferSize);
			offset += vertexBufferSize;
		}

		return buffer;
	}

	std::unique_ptr<blMeshStorage::index[]> blMeshStorage::CreateIndexBuffer(std::vector<const blMeshStorage*> storageArray)
	{
		size_t totalBufferSize = 0;

		for (const auto& storage : storageArray)
		{
			totalBufferSize += storage->GetHeader().GetIndexBufferSize();
		}

		std::unique_ptr<blMeshStorage::index[]> buffer = std::make_unique<blMeshStorage::index[]>(totalBufferSize);

		size_t offset = 0;
		for (size_t i = 0; i < storageArray.size(); ++i)
		{
			const blMeshStorage* storage = storageArray[i];
			const size_t indexBufferSize = storage->GetHeader().GetIndexBufferSize();
			memcpy(buffer.get() + offset, storage->mIndexDataStart, indexBufferSize);
			offset += indexBufferSize;
		}

		return buffer;
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
		size_t vertexSize,
		BufferFormat format) : blMeshStorage()
	{
		const Header header = Header(vertexCount, indexCount, vertexSize, format);
		const size_t bufferSize = GetBufferSize(header);
		mBuffer = std::make_unique<byte[]>(bufferSize);
		GetHeaderMutable() = header;
		InitializeIndexingPointers();
	}

	blMeshStorage::blMeshStorage(
		byte vertices[],
		size_t vertexSize,
		BufferFormat format) : blMeshStorage((size_t)(sizeof(vertices) / vertexSize), 0, vertexSize, format)
	{
		memcpy(mVertexDataStart, vertices, sizeof(vertices));
	}

	blMeshStorage::blMeshStorage(
		const byte* vertices,
		const size_t vertexCount,
		const index* indices,
		const size_t indexCount,
		const size_t vertexSize,
		BufferFormat format) : blMeshStorage(
			vertexCount,
			indexCount,
			vertexSize,
			format)
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
		mIndexDataStart = reinterpret_cast<blMeshStorage::index*>(mBuffer.get() + sizeof(Header));
		mVertexDataStart = reinterpret_cast<byte*>(mIndexDataStart + GetIndexCount());
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

	const BufferFormat& blMeshStorage::GetFormat() const
	{
		return GetHeader().mFormat;
	}

	const void* blMeshStorage::GetVertex(size_t index) const
	{
		return mVertexDataStart + index * GetHeader().mVertexSize;
	}

	void* blMeshStorage::GetVertexMutable(size_t vertex)
	{
		return mVertexDataStart + vertex * GetHeader().mVertexSize;
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

	void blMeshStorage::GraftMeshData(
		const blMeshStorage& other, const size_t index, const size_t vertex, bool offsetVertexIndices)
	{
		if (index + other.GetIndexCount() > GetIndexCount() || vertex + other.GetVertexCount() > GetVertexCount())
		{
			throw std::out_of_range("GraftMeshData: The provided index and vertex offsets exceed the bounds of the current mesh storage.");
		}

		memcpy(
			&GetIndexMutable(index), 
			other.mIndexDataStart,
			other.GetIndexCount() * sizeof(blMeshStorage::index));

		memcpy(
			GetVertexMutable(vertex), 
			other.mVertexDataStart,
			other.GetVertexCount() * GetVertexSize());

		//I thought I needed to do this. but the vertex buffer view is already offset. So the indices don't need to be. 
		//but maybe there are other render scenarios where we do want these offset? let's keep the option here. 
		if (offsetVertexIndices)
		{
			for (blMeshStorage::index* i = &GetIndexMutable(index);
				i != &GetIndexMutable(index + other.GetIndexCount());
				++i)
			{
				//after the graft is complete, we need to shift over all of the indices
				*i += vertex;
			}
		}
	}

	const std::vector<VertexElementDescription> StandardVertexDefinition::Description =
	{
		{ "Position",	VertexElementType::Float3 },
		{ "Normal",		VertexElementType::Float3 },
		{ "Tangent",	VertexElementType::Float3 },
		{ "Color",		VertexElementType::Float4 },
		{ "TEXCOORD",	VertexElementType::Float2 }
	};

	void blInlineMesh::WriteToStream(blResourceStream& stream) const
	{
		stream.AllocateExplicit(mHeader.GetVertexBufferSize());
		stream.AllocateExplicit(mHeader.GetIndexBufferSize());
	}

	const blMeshStorage::index* blInlineMesh::GetIndicesStart() const
	{
		return reinterpret_cast<const blMeshStorage::index*>(
			reinterpret_cast<const byte*>(&mHeader) + sizeof(Header));
	}

	blMeshStorage::index* blInlineMesh::GetIndicesStartMutable()
	{
		return reinterpret_cast<blMeshStorage::index*>(
			reinterpret_cast<byte*>(&mHeader) + sizeof(Header));
	}

	uint64_t blInlineMesh::GetTotalSize() const
	{
		return sizeof(Header) +
			(mHeader.mVertexCount * mHeader.mVertexSize) +
			(mHeader.mIndexCount * sizeof(blMeshStorage::index));
	}

	const byte* blInlineMesh::GetVertexStart() const
	{
		return reinterpret_cast<const byte*>(GetIndicesStart()) + sizeof(blMeshStorage::index) * mHeader.mIndexCount;
	}

	byte* blInlineMesh::GetVertexStartMutable() 
	{
		return reinterpret_cast<byte*>(GetIndicesStartMutable()) + sizeof(blMeshStorage::index) * mHeader.mIndexCount;
	}

	// Constructors
	blInlineMesh::blInlineMesh()
	{
		mHeader = Header();
	}

	blInlineMesh::blInlineMesh(size_t vertexCount, size_t indexCount, size_t vertexSize, BufferFormat format, blResourceRef description)
	{
		mHeader = Header(vertexCount, indexCount, vertexSize, format);
		mHeader.description = description;
	}

	void SubdividePrototype(MeshDataPrototype& prototype)
	{
		struct Triangle
		{
			union
			{
				struct
				{
					uint16_t i0;
					uint16_t i1;
					uint16_t i2;
				};

				uint16_t data[3];
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
	}
}

namespace BoulderLeaf::Graphics
{
	blIndexedMeshResource::blIndexedMeshResource(
		blResourceRefOfType<blListResource> indexListRef,
		blResourceRefOfType<blArrayBufferResource> arrayBufferResourceRef)
		: mIndexListRef(indexListRef),
		mArrayBufferResourceRef(arrayBufferResourceRef)
	{

	}

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint32_t indexCount)
	{
		blResourceHandleOfType<blArrayBufferResource> vertexArrayResource = CreateArrayBufferResource(
			resourceContainer,
			name,
			descriptionResourceRef,
			vertexSize,
			vertexCount
		);

		blResourceHandleOfType<blListResource> indexArrayResource = resourceContainer->CreateResourceOfTypeWithDynamicSize<blListResource>(
			name + L"Data",
			indexCount,
			sizeof(std::uint16_t)
		);

		blResourceHandleOfType<blIndexedMeshResource> meshResource = resourceContainer->CreateResourceOfType<blIndexedMeshResource>(
			name,
			indexArrayResource.GetRef(),
			vertexArrayResource.GetRef()
		);

		return meshResource;
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		const void* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount,
		const std::uint16_t* indices,
		uint32_t indexCount)
	{
		blResourceHandleOfType<blArrayBufferResource> vertexArrayResource = CreateArrayBufferResource(
			resourceContainer,
			name,
			descriptionResourceRef,
			vertices,
			vertexSize,
			vertexCount
		);

		blResourceHandleOfType<blListResource> indexArrayResource = resourceContainer->CreateResourceOfTypeWithDynamicSize<blListResource>(
			name + L"Data",
			indexCount,
			sizeof(std::uint16_t),
			indices
		);

		blResourceHandleOfType<blIndexedMeshResource> meshResource = resourceContainer->CreateResourceOfType<blIndexedMeshResource>(
			name,
			indexArrayResource.GetRef(),
			vertexArrayResource.GetRef()
		);

		return meshResource;
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceHandleOfType<blIndexedMeshResource> sourceMeshHandle,
		const blBufferElementAdapter& adapter)
	{
		blResourceContainer* resourceContainer = sourceMeshHandle.GetContainer();
		const blIndexedMeshResource& sourceMesh = *sourceMeshHandle;

		const blArrayBufferResource& sourceArrayBuffer =
			*resourceContainer->CreateHandleFromRefOfType<blArrayBufferResource>(sourceMesh.mArrayBufferResourceRef);

		blResourceHandleOfType<blArrayBufferResource> vertexArrayResource = CreateArrayBufferResource(
			resourceContainer,
			std::wstring(sourceMeshHandle.GetName()) + L" Translated Array",
			sourceArrayBuffer,
			adapter
		);

		blResourceHandleOfType<blIndexedMeshResource> meshResource = resourceContainer->CreateResourceOfType<blIndexedMeshResource>(
			std::wstring(sourceMeshHandle.GetName()) + L" Translated",
			blResourceRefOfType<blListResource>(sourceMesh.mIndexListRef.GetId()),
			vertexArrayResource.GetRef()
		);

		return meshResource;
	}
}