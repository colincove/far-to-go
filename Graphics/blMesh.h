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
#include <string>
#include <blBuffer.h>
#include <Resources/blResources.h>
#include <Resources/blResourceHandleOfType.h>
#include <blVertex.h>

namespace BoulderLeaf::Graphics
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

	struct Quad
	{
		union
		{
			struct
			{
				uint16_t i0;
				uint16_t i1;
				uint16_t i2;
				uint16_t i3;
			};

			uint16_t data[4];
		};

		void Write(uint16_t* indices, uint16_t offset) const
		{
			indices[0] = i0 + offset;
			indices[1] = i2 + offset;
			indices[2] = i1 + offset;
			indices[3] = i2 + offset;
			indices[4] = i3 + offset;
			indices[5] = i1 + offset;
		}
	};

	struct MeshDataPrototype
	{
		std::vector<StandardVertex> vertices;
		std::vector<uint16_t> indices;
	};

	void SubdividePrototype(MeshDataPrototype& prototype);

	inline bool operator==(const StandardVertex& lhs, const StandardVertex& rhs)
	{
		return lhs.Colour == rhs.Colour && lhs.Normal == rhs.Normal && lhs.Position == rhs.Position && lhs.UV == rhs.UV;
	}

	inline bool operator!=(const StandardVertex& lhs, const StandardVertex& rhs) { return !(lhs == rhs); }
}

namespace BoulderLeaf::Graphics
{
	struct blIndexedMeshResource : blBaseResource
	{
		blResourceRefOfType<blListResource> mIndexListRef;
		blResourceRefOfType<blArrayBufferResource> mArrayBufferResourceRef;

		blIndexedMeshResource(
			blResourceRefOfType<blListResource> indexListRef,
			blResourceRefOfType<blArrayBufferResource> arrayBufferResourceRef);
	};

	struct blIndexedMeshResourceExpanded
	{
		blResourceContainer* container;
		blResourceHandleOfType<blIndexedMeshResource> indexedMesh;
		blResourceHandleOfType<blArrayBufferResource> arrayBuffer;
		blResourceHandleOfType<blListResource> vertexBuffer;
		blResourceHandleOfType<blBufferDescriptionResource> bufferDescriptionResource;
		blResourceHandleOfType<blListResource> indexBuffer;
	};

	blIndexedMeshResourceExpanded ExpandIndexedMeshResource(blResourceHandleOfType<blIndexedMeshResource> resource);

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint32_t indexCount);

	template<typename TVertex>
	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		uint32_t vertexCount,
		uint32_t indexCount)
	{
		return CreateIndexedMeshResource(
			resourceContainer,
			name,
			descriptionResourceRef,
			sizeof(TVertex),
			vertexCount,
			indexCount);
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		const void* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount,
		const std::uint16_t* indices,
		uint32_t indexCount);

	template<typename TVertex>
	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		const TVertex* vertices,
		uint32_t vertexCount,
		const std::uint16_t* indices,
		uint32_t indexCount)
	{
		return CreateIndexedMeshResource(
			resourceContainer,
			name,
			descriptionResourceRef,
			vertices,
			sizeof(TVertex),
			vertexCount,
			indices,
			indexCount
		);
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceHandleOfType<blIndexedMeshResource> sourceMeshHandle,
		const blBufferElementAdapter& adapter);

	Math::Vector4 ComputePositionForHeightMapVertex(
		const uint32_t& sizeOfHeightMap,
		const uint32_t& vertexId,
		const float& heightValue
	);

	Math::Vector4 ComputeNormal(
		const Math::Vector4& p0,
		const Math::Vector4& p1,
		const Math::Vector4& p2);

	void CalculateVertexNormals(blResourceHandleOfType<blIndexedMeshResource> sourceMeshHandle);

	void CalculateVertexNormals(
		const uint16_t* indices,
		uint32_t indexCount,
		byte* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint64_t normalPropertyOffset,
		uint64_t positionPropertyOffset);

	void CalculateVertexNormalsForHeightMap(const uint32_t sizeOfHeightMap, blResourceHandleOfType<blIndexedMeshResource> sourceMeshHandle);

	void CalculateVertexNormalsForHeightMap(
		const uint32_t sizeOfHeightMap,
		const uint16_t* indices,
		uint32_t indexCount,
		byte* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint64_t normalPropertyOffset,
		uint64_t heightPropertyOffset);
}