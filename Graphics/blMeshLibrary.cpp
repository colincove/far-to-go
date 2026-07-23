#pragma once

#include <blMeshLibrary.h>
#include <blVector3.inl>
#include <blColour.h>
#include <array>
#include <cstdint>
#include <blMath.inl>
#include <blMesh.h>

namespace BoulderLeaf::Graphics
{
	blResourceHandleOfType<blBufferDescriptionResource> BufferDescriptionResource(blResourceContainer* container, const BufferDescription& description)
	{
		return container->CreateResourceOfTypeWithDynamicSize<blBufferDescriptionResource>(L"", description);
	}

	blResourceHandleOfType<blIndexedMeshResource> CubeResource(blResourceContainer* container)
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

		static std::array<uint16_t, 36> indices =
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

		blResourceHandleOfType<blBufferDescriptionResource> desc = BufferDescriptionResource(
			container, { BufferFormat::BoulderLeaf, StandardVertexDefinition::Description });

		return CreateIndexedMeshResource<StandardVertex>(
			container,
			L"Cube",
			desc.GetRef(),
			vertices.data(),
			(uint32_t) vertices.size(),
			indices.data(),
			(uint32_t) indices.size()
		);
	}

	namespace
	{
		void BuildCylinderTopCap(
			float bottomRadius,
			float topRadius,
			float height,
			unsigned int sliceCount,
			unsigned int stackCount,
			MeshDataPrototype& meshData)
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
				meshData.indices.push_back((uint16_t)centerIndex);
				meshData.indices.push_back((uint16_t)(baseIndex + i + 1));
				meshData.indices.push_back((uint16_t)(baseIndex + i));
			}
		}

		MeshDataPrototype CreateGeosphereData(float radius, unsigned int numSubdivisions)
		{
			MeshDataPrototype meshData;

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
				SubdividePrototype(meshData);
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

			return meshData;
		}

		MeshDataPrototype CreateCylinderData(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount)
		{
			MeshDataPrototype meshData;

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
					meshData.vertices.push_back(vertex);
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
					meshData.indices.push_back((uint16_t)(i * ringVertexCount + j));
					meshData.indices.push_back((uint16_t)((i + 1) * ringVertexCount + j));
					meshData.indices.push_back((uint16_t)((i + 1) * ringVertexCount + j + 1));
					meshData.indices.push_back((uint16_t)(i * ringVertexCount + j));
					meshData.indices.push_back((uint16_t)((i + 1) * ringVertexCount + j + 1));
					meshData.indices.push_back((uint16_t)(i * ringVertexCount + j + 1));
				}
			}

			BuildCylinderTopCap(
				bottomRadius,
				topRadius,
				height,
				sliceCount,
				stackCount,
				meshData);

			return meshData;
		}
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateCylinderResource(blResourceContainer* container, float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount)
	{
		const MeshDataPrototype meshData = CreateCylinderData(
			bottomRadius, topRadius, height, sliceCount, stackCount);

		blResourceHandleOfType<blBufferDescriptionResource> desc = BufferDescriptionResource(
			container, { BufferFormat::BoulderLeaf, StandardVertexDefinition::Description });

		return CreateIndexedMeshResource<StandardVertex>(
			container,
			L"Cylinder",
			desc.GetRef(),
			meshData.vertices.data(),
			(uint32_t)meshData.vertices.size(),
			meshData.indices.data(),
			(uint32_t)meshData.indices.size()
		);
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateGeosphereResource(blResourceContainer* container, float radius, unsigned int numSubdivisions)
	{
		const MeshDataPrototype meshData = CreateGeosphereData(radius, numSubdivisions);

		blResourceHandleOfType<blBufferDescriptionResource> desc = BufferDescriptionResource(
			container, { BufferFormat::BoulderLeaf, StandardVertexDefinition::Description });

		return CreateIndexedMeshResource<StandardVertex>(
			container,
			L"Geosphere",
			desc.GetRef(),
			meshData.vertices.data(),
			(uint32_t)meshData.vertices.size(),
			meshData.indices.data(),
			(uint32_t)meshData.indices.size()
		);
	}

	blResourceHandleOfType<blIndexedMeshResource> CreateTerrainResource(blResourceContainer* container, uint16_t size)
	{
		BufferDescription desc = 
		{ 
			BufferFormat::BoulderLeaf, 
			blTerrainVertexElementDescriptions
		};

		blResourceHandleOfType<blBufferDescriptionResource> descResource = BufferDescriptionResource(container, desc);
		blResourceHandleOfType<blIndexedMeshResource> meshResource = CreateIndexedMeshResource<blTerrainVertex>(
			container,
			L"Terrain",
			descResource.GetRef(),
			(size + 1) * (size  + 1),
			size * size * 6
		);

		blResourceHandleOfType<blArrayBufferResource> vertexArrayBuffer =
			container->CreateHandleFromRefOfType<blArrayBufferResource>(meshResource->mArrayBufferResourceRef);
		blResourceHandleOfType<blListResource> vertexBuffer =
			container->CreateHandleFromRefOfType<blListResource>(vertexArrayBuffer->mBufferResourceRef);

		blResourceHandleOfType<blListResource> indexBuffer =
			container->CreateHandleFromRefOfType<blListResource>(meshResource->mIndexListRef);

		for (blTerrainVertex& vertexData : blListResource::Iterator<blTerrainVertex>(vertexBuffer.GetMutable()))
		{
			vertexData.Height = 0.0f;
		}

		const uint32_t sizePlusOne = size + 1;
		for(uint32_t x = 0; x < size; ++x)
		{
			for(uint32_t y = 0; y < size; ++y)
			{
				const uint32_t topLeft = x + y * sizePlusOne;
				const uint32_t topRight = x + y * sizePlusOne + 1;
				Quad quad = { 
					topLeft,
					topRight,
					topLeft + sizePlusOne,
					topRight + sizePlusOne};

				quad.Write(&indexBuffer->GetMutable<uint16_t>((x * 6) + (y * size * 6)), 0);
			}
		}

		CalculateVertexNormalsForHeightMap(size, meshResource);
		return meshResource;
	}
}