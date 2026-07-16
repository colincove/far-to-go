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

	blResourceHandleOfType<blIndexedMeshResource> CreateIndexedMeshResource(
		blResourceContainer* resourceContainer,
		std::wstring name,
		blResourceRefOfType<blBufferDescriptionResource> descriptionResourceRef,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint32_t indexCount);

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
}