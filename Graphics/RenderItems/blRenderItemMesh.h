#pragma once

#include <blMesh.h>
#include <blMaterial.h>
#include <blBuffer.h>
#include <blBufferTypes.h>
#include <blMatrix4x4.inl>
#include <memory>
#include <vector>
#include <RenderItems/blRenderItem.h>
#include <blCompositeMesh.h>
#include <Resources/blResources.h>
#include <Resources/blResourceHandleOfType.h>

namespace BoulderLeaf::Graphics
{
	struct RenderMeshData : public RenderData
	{
		Math::Matrix4x4 transform;
	};

	struct RenderMeshDataInstanced : public RenderData
	{
		//GPU buffer resource containing instance data. This is what is actually bound to the pipeline.
		blResourceHandleOfType<blArrayBufferResource> constantBuffer;
		blResourceHandleOfType<blIndexedMeshResource> mesh;
		blResourceHandleOfType<blMaterialResource> material;
		blResourceHandleOfType<blConstantBufferResource> mConstantBuffers;
	};

	// Copy of RenderMeshDataInstanced for V2 render path. This variant uses the
	// consolidated constantBuffers resource which owns the instance buffer and
	// pass constant buffers.
	struct RenderMeshDataInstancedV2 : public RenderData
	{
		blResourceHandleOfType<blIndexedMeshResource> mesh;
		blResourceHandleOfType<blMaterialResource> material;
		// Consolidated constant buffer resource: contains instance buffer and pass constant buffers.
		blResourceHandleOfType<blConstantBufferResource> constantBuffers;
	};

	struct RenderCompositeMeshDataInstanced : public RenderData
	{
		//GPU buffer resource containing instance data. This is what is actually bound to the pipeline.
		blResourceHandleOfType<blArrayBufferResource> constantBuffer;
		blResourceHandleOfType<blCompositeMeshResource> compositeMesh;
		blResourceHandleOfType<blMaterialResource> material;
		BoulderLeaf::blResourceId submeshId;
		size_t instance;
	};

	struct RenderCompositeMeshDataWithPassConstants : public RenderData
	{
		//GPU buffer resource containing instance data. This is what is actually bound to the pipeline.
		blResourceHandleOfType<blArrayBufferResource> instanceConstantBuffer;
		blResourceHandleOfType<blArrayBufferResource> passConstantBuffer;
		blResourceHandleOfType<blCompositeMeshResource> compositeMesh;
		blResourceHandleOfType<blMaterialResource> material;
		std::vector<BoulderLeaf::blResourceId> submeshIds;
		size_t instance;
		blResourceHandleOfType<blConstantBufferResource> mConstantBuffers;
	};
}