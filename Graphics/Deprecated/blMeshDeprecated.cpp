#include <Deprecated/blMeshDeprecated.h>
#include <blVector3.inl>
#include <array>
#include <blColour.h>

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
}

namespace BoulderLeaf::Graphics
{
	namespace
	{
		void BuildCylinderTopCap(
			float bottomRadius,
			float topRadius,
			float height,
			unsigned int sliceCount,
			unsigned int stackCount,
			StandardMesh::Prototype& meshData)
		{
			unsigned int baseIndex = (unsigned int)meshData.vertices.size();
			const Math::Vector4 Normal(Math::Vector4::Vector3D(0.0f, 1.0f, 0.0f));
			float y = 0.5f * height;
			float dTheta = 2.0f * PIf / sliceCount;
			// Duplicate cap ring vertices because the texture coordinates and
			// normals differ.
			for (unsigned int i = 0; i <= sliceCount; ++i)
			{
				float x = topRadius * cosf(i * dTheta);
				float z = topRadius * sinf(i * dTheta);
				// Scale down by the height to try and make top cap texture coord
				// area proportional to base.
				float u = x / height + 0.5f;
				float v = z / height + 0.5f;
				meshData.vertices.push_back(
					StandardVertex(
						Math::Vector3(x, y, z),
						Normal,
						Math::Vector3(1.0f, 0.0f, 0.0f),
						Math::Vector4(),
						Math::Vector2(u, v)));
			}
			// Cap center vertex.
			meshData.vertices.push_back(
				StandardVertex(
					Math::Vector3(0.0f, y, 0.0f),
					Normal,
					Math::Vector3(1.0f, 0.0f, 0.0f),
					Math::Vector4(),
					Math::Vector2(0.5f, 0.5f)));

			// Index of center vertex.
			unsigned int centerIndex = (unsigned int)meshData.vertices.size() - 1;
			for (unsigned int i = 0; i < sliceCount; ++i)
			{
				meshData.indices.push_back((blMeshStorage::index)centerIndex);
				meshData.indices.push_back((blMeshStorage::index)(baseIndex + i + 1));
				meshData.indices.push_back((blMeshStorage::index)(baseIndex + i));
			}
		}
	}

	const StandardMesh& Cube()
	{
		using namespace Math;

		static std::array<StandardVertex, 8> vertices =
		{
			StandardVertex({ Vector3(-1.0f, -1.0f, -1.0f), Vector4(), Vector3(), Vector4(Colours::White), Vector2() }),
			StandardVertex({ Vector3(-1.0f, +1.0f, -1.0f), Vector4(), Vector3(), Vector4(Colours::Black), Vector2()}),
			StandardVertex({ Vector3(+1.0f, +1.0f, -1.0f), Vector4(), Vector3(), Vector4(Colours::Red), Vector2() }),
			StandardVertex({ Vector3(+1.0f, -1.0f, -1.0f), Vector4(), Vector3(), Vector4(Colours::Green), Vector2() }),
			StandardVertex({ Vector3(-1.0f, -1.0f, +1.0f), Vector4(), Vector3(), Vector4(Colours::Blue), Vector2() }),
			StandardVertex({ Vector3(-1.0f, +1.0f, +1.0f), Vector4(), Vector3(), Vector4(Colours::Yellow), Vector2() }),
			StandardVertex({ Vector3(+1.0f, +1.0f, +1.0f), Vector4(), Vector3(), Vector4(Colours::Cyan), Vector2() }),
			StandardVertex({ Vector3(+1.0f, -1.0f, +1.0f), Vector4(), Vector3(), Vector4(Colours::Magenta), Vector2()
		})
		};

		static std::array<blMeshStorage::index, 36> indices =
		{
			// front face
			0, 1, 2,
			0, 2, 3,
			// back face
			4, 6, 5,
			4, 7, 6,
			// left face
			4, 5, 1,
			4, 1, 0,
			// right face
			3, 2, 6,
			3, 6, 7,
			// top face
			1, 5, 6,
			1, 6, 2,
			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		static StandardMesh cube(vertices.data(), vertices.size(), indices.data(), indices.size());

		return cube;
	}

	StandardMesh CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount)
	{
		StandardMesh::Prototype& prototype = StandardMesh::GetPrototype();

		float stackHeight = height / stackCount;
		// Amount to increment radius as we move up each stack level from
		// bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;
		unsigned int ringCount = stackCount + 1;

		// Compute vertices for each stack ring starting at the bottom and
		// moving up.
		for (unsigned int i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;
			// vertices of ring
			float dTheta = 2.0f * PIf / sliceCount;
			for (unsigned int j = 0; j <= sliceCount; ++j)
			{
				StandardVertex vertex;
				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);
				vertex.Position = Math::Vector3(r * c, y, r * s);
				vertex.UV.x = (float)j / sliceCount;
				vertex.UV.y = 1.0f - (float)i / stackCount;
				vertex.Tangent = Math::Vector3(-s, 0.0f, c);
				float dr = bottomRadius - topRadius;
				Math::Vector3 bitangent(dr * c, -height, dr * s);
				vertex.Normal = Math::Vector4::Vector3D(vertex.Tangent.Cross(bitangent).Normalize());
				prototype.vertices.push_back(vertex);
			}
		}

		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		unsigned int ringVertexCount = sliceCount + 1;
		// Compute indices for each stack.
		for (unsigned int i = 0; i < stackCount; ++i)
		{
			for (unsigned int j = 0; j < sliceCount; ++j)
			{
				prototype.indices.push_back((blMeshStorage::index)(i * ringVertexCount + j));
				prototype.indices.push_back((blMeshStorage::index)((i + 1) * ringVertexCount + j));
				prototype.indices.push_back((blMeshStorage::index)((i + 1) * ringVertexCount + j + 1));
				prototype.indices.push_back((blMeshStorage::index)(i * ringVertexCount + j));
				prototype.indices.push_back((blMeshStorage::index)((i + 1) * ringVertexCount + j + 1));
				prototype.indices.push_back((blMeshStorage::index)(i * ringVertexCount + j + 1));
			}
		}

		BuildCylinderTopCap(
			bottomRadius,
			topRadius,
			height,
			sliceCount,
			stackCount,
			prototype);

		return StandardMesh(prototype);
	}

	StandardMesh CreateGeosphere(float radius, unsigned int numSubdivisions)
	{
		StandardMesh::Prototype& meshData = StandardMesh::GetPrototype();

		// Put a cap on the number of subdivisions.
		numSubdivisions = std::min<unsigned int>(numSubdivisions, 6u);
		// Approximate a sphere by tessellating an icosahedron.
		const float X = 0.525731f;
		const float Z = 0.850651f;

		Math::Vector3 pos[12] =
		{
			Math::Vector3(-X, 0.0f, Z), Math::Vector3(X, 0.0f, Z),
			Math::Vector3(-X, 0.0f, -Z), Math::Vector3(X, 0.0f, -Z),
			Math::Vector3(0.0f, Z, X), Math::Vector3(0.0f, Z, -X),
			Math::Vector3(0.0f, -Z, X), Math::Vector3(0.0f, -Z, -X),
			Math::Vector3(Z, X, 0.0f), Math::Vector3(-Z, X, 0.0f),
			Math::Vector3(Z, -X, 0.0f), Math::Vector3(-Z, -X, 0.0f)
		};

		unsigned int k[60] =
		{
		1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
		1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
		};
		meshData.vertices.resize(12);
		meshData.indices.assign(&k[0], &k[60]);
		for (unsigned int i = 0; i < 12; ++i)
		{
			meshData.vertices[i].Position = pos[i];
		}

		for (unsigned int i = 0; i < numSubdivisions; ++i)
		{
			SubdividePrototype<StandardMesh::TVertexDef>(meshData);
		}

		// Project vertices onto sphere and scale.
		for (unsigned int i = 0; i < meshData.vertices.size(); ++i)
		{
			// Project onto unit sphere.
			Math::Vector3 n = meshData.vertices[i].Position.Normalize();

			// Project onto sphere.
			Math::Vector3 p = n * radius;
			meshData.vertices[i].Position = p;
			meshData.vertices[i].Normal = Math::Vector4::Vector3D(n);
			// Derive texture coordinates from spherical coordinates.
			float theta = atan2f(meshData.vertices[i].Position.z, meshData.vertices[i].Position.x);
			// Put in [0, 2pi].
			if (theta < 0.0f)
				theta += PIf;
			float phi = acosf(meshData.vertices[i].Position.y / radius);
			meshData.vertices[i].UV.x = theta / PIf;
			meshData.vertices[i].UV.y = phi / PIf;
			// Partial derivative of P with respect to theta
			meshData.vertices[i].Tangent.x = -radius * sinf(phi) * sinf(theta);
			meshData.vertices[i].Tangent.y = 0.0f;
			meshData.vertices[i].Tangent.z = +radius * sinf(phi) * cosf(theta);
			meshData.vertices[i].Tangent = meshData.vertices[i].Tangent.Normalize();
		}

		return StandardMesh(meshData);
	}
}