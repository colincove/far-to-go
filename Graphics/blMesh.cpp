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

	blIndexedMeshResourceExpanded ExpandIndexedMeshResource(blResourceHandleOfType<blIndexedMeshResource> resource)
	{
		blIndexedMeshResourceExpanded result;
		result.container = resource.GetContainer();
		result.arrayBuffer = result.container->CreateHandleFromRefOfType<blArrayBufferResource>(resource->mArrayBufferResourceRef);
		result.vertexBuffer = result.container->CreateHandleFromRefOfType<blListResource>(result.arrayBuffer->mBufferResourceRef);
		result.bufferDescriptionResource = result.container->CreateHandleFromRefOfType<blBufferDescriptionResource>(result.arrayBuffer->mDescriptionResourceRef);
		result.indexBuffer = result.container->CreateHandleFromRefOfType<blListResource>(resource->mIndexListRef);
		return result;
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

		CalculateVertexNormals(meshResource);

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

	Math::Vector4 ComputePositionForHeightMapVertex(
		const uint32_t& sizeOfHeightMap,
		const uint32_t& vertexId,
		const float& heightValue)
	{
		const uint32_t sizePlusOne = sizeOfHeightMap + 1;
		const uint32_t vertexIdPlusSize = vertexId + sizePlusOne;
		const uint32_t moduloResult = vertexIdPlusSize % sizePlusOne;

		return Math::Vector4::Point3D(
			moduloResult,
			heightValue,
			(float) round(vertexId / sizePlusOne));
	}

	Math::Vector4 ComputeNormal(
		const Math::Vector4& p0,
		const Math::Vector4& p1,
		const Math::Vector4& p2)
	{
		Math::Vector4 u = p1 - p0;
		Math::Vector4 v = p2 - p0;

		return u.Cross(v).Normalize();
	}

	void CalculateVertexNormals(blResourceHandleOfType<blIndexedMeshResource> sourceMeshHandle)
	{
		blIndexedMeshResourceExpanded expandedResource = ExpandIndexedMeshResource(sourceMeshHandle);
		BufferDescription desc = expandedResource.bufferDescriptionResource->GetBufferDescription();

		CalculateVertexNormals(
			&expandedResource.indexBuffer->Get<uint16_t>(0),
			expandedResource.indexBuffer->mCount,
			&expandedResource.vertexBuffer->GetMutable<byte>(0),
			expandedResource.vertexBuffer->mElementSize,
			expandedResource.vertexBuffer->mCount,
			CalculateBufferElementsSemanticOffset(desc.elementDescriptions, BufferElementSemantic::Normal, blDefaultBufferElementFormatter::Get()),
			CalculateBufferElementsSemanticOffset(desc.elementDescriptions, BufferElementSemantic::Position, blDefaultBufferElementFormatter::Get())
		);
	}

	void CalculateVertexNormals(
		const uint16_t* indices,
		uint32_t indexCount,
		byte* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint64_t normalPropertyOffset,
		uint64_t positionPropertyOffset)
	{
		//reset all normals to start from zero
		for (uint32_t vertexId = 0; vertexId < vertexCount; ++vertexId)
		{
			Math::Vector4& normal = *reinterpret_cast<Math::Vector4*>(vertices + (normalPropertyOffset + (vertexSize * vertexId)));
			normal = Math::Vector4::Zero();
		}

		for (uint16_t i = 0; i < indexCount; i+=3)
		{
			const Triangle& face = *reinterpret_cast<const Triangle*>(indices + i);

			Math::Vector4& p0 = *reinterpret_cast<Math::Vector4*>(positionPropertyOffset + vertices + (vertexSize * face.i0));
			Math::Vector4& p1 = *reinterpret_cast<Math::Vector4*>(positionPropertyOffset + vertices + (vertexSize * face.i1));
			Math::Vector4& p2 = *reinterpret_cast<Math::Vector4*>(positionPropertyOffset + vertices + (vertexSize * face.i2));

			Math::Vector4& n0 = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * face.i0));
			Math::Vector4& n1 = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * face.i1));
			Math::Vector4& n2 = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * face.i2));

			Math::Vector4 faceNormal = ComputeNormal(p0, p1, p2);

			n0 += faceNormal;
			n1 += faceNormal;
			n2 += faceNormal;
		}

		for (uint32_t vertexId = 0; vertexId < vertexCount; ++vertexId)
		{
			Math::Vector4& normal = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * vertexId));
			normal = normal.Normalize();
		}
	}

	void CalculateVertexNormalsForHeightMap(const uint32_t sizeOfHeightMap, blResourceHandleOfType<blIndexedMeshResource> sourceMeshHandle)
	{
		blIndexedMeshResourceExpanded expandedResource = ExpandIndexedMeshResource(sourceMeshHandle);
		BufferDescription desc = expandedResource.bufferDescriptionResource->GetBufferDescription();

		CalculateVertexNormalsForHeightMap(
			sizeOfHeightMap,
			&expandedResource.indexBuffer->Get<uint16_t>(0),
			expandedResource.indexBuffer->mCount,
			&expandedResource.vertexBuffer->GetMutable<byte>(0),
			expandedResource.vertexBuffer->mElementSize,
			expandedResource.vertexBuffer->mCount,
			CalculateBufferElementsSemanticOffset(desc.elementDescriptions, BufferElementSemantic::Normal, blDefaultBufferElementFormatter::Get()),
			CalculateBufferElementsSemanticOffset(desc.elementDescriptions, BufferElementSemantic::Height, blDefaultBufferElementFormatter::Get())
		);
	}

	void CalculateVertexNormalsForHeightMap(
		const uint32_t sizeOfHeightMap,
		const uint16_t* indices,
		uint32_t indexCount,
		byte* vertices,
		uint64_t vertexSize,
		uint32_t vertexCount,
		uint64_t normalPropertyOffset,
		uint64_t heightPropertyOffset)
	{
		//reset all normals to start from zero
		for (uint32_t vertexId = 0; vertexId < vertexCount; ++vertexId)
		{
			Math::Vector4& normal = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * vertexId));
			normal = Math::Vector4::Zero();
		}

		for (uint16_t i = 0; i < indexCount; i += 3)
		{
			const Triangle& face = *reinterpret_cast<const Triangle*>(indices + i);

			float& h0 = *reinterpret_cast<float*>(heightPropertyOffset + vertices + (vertexSize * face.i0));
			float& h1 = *reinterpret_cast<float*>(heightPropertyOffset + vertices + (vertexSize * face.i1));
			float& h2 = *reinterpret_cast<float*>(heightPropertyOffset + vertices + (vertexSize * face.i2));

			Math::Vector4 p0 = ComputePositionForHeightMapVertex(sizeOfHeightMap, face.i0, h0);
			Math::Vector4 p1 = ComputePositionForHeightMapVertex(sizeOfHeightMap, face.i1, h1);
			Math::Vector4 p2 = ComputePositionForHeightMapVertex(sizeOfHeightMap, face.i2, h2);

			Math::Vector4& n0 = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * face.i0));
			Math::Vector4& n1 = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * face.i1));
			Math::Vector4& n2 = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * face.i2));

			Math::Vector4 faceNormal = ComputeNormal(p0, p1, p2);

			n0 += faceNormal;
			n1 += faceNormal;
			n2 += faceNormal;
		}

		for (uint32_t vertexId = 0; vertexId < vertexCount; ++vertexId)
		{
			Math::Vector4& normal = *reinterpret_cast<Math::Vector4*>(normalPropertyOffset + vertices + (vertexSize * vertexId));
			normal = normal.Normalize();
		}
	}
}