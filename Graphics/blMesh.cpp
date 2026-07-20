#include <blMesh.h>
#include <blVector3.inl>
#include <array>

namespace BoulderLeaf::Graphics
{

	void SubdividePrototype(MeshDataPrototype& prototype)
	{
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
			std::array<uint16_t, 3> newIndices =
			{
				static_cast<uint16_t>(newVertexSize - 3),
				static_cast<uint16_t>(newVertexSize - 2),
				static_cast<uint16_t>(newVertexSize - 1)
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