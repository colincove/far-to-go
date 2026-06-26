#pragma once

#include <blMesh.h>
#include <blMaterial.h>
#include <blBuffer.h>
#include <blBufferTypes.h>
#include <Resources/blResource.h>
#include <blMatrix4x4.inl>
#include <memory>
#include <vector>
#include <RenderItems/blRenderItem.h>
#include <blCompositeMesh.h>

namespace BoulderLeaf::Graphics
{
	struct RenderMeshData : public RenderData
	{
		Math::Matrix4x4 transform;
		blDataBufferInterfaceResourcePtr constantBuffer;
		blMeshBaseResourceWeakPtr mesh;
		blMaterialResourceWeakPtr material;
	};

	struct RenderMeshDataInstanced : public RenderData
	{
		//GPU buffer resource containing instance data. This is what is actually bound to the pipeline.
		blDataBufferInterfaceResourcePtr constantBuffer;
		blMeshBaseResourcePtr mesh;
		blMaterialResourcePtr material;
	};

	struct RenderCompositeMeshDataInstanced : public RenderData
	{
		//GPU buffer resource containing instance data. This is what is actually bound to the pipeline.
		blDataBufferInterfaceResourcePtr constantBuffer;
		blCompositeMeshResourcePtr compositeMesh;
		blMaterialResourcePtr material;
		blResourceId submeshId;
		size_t instance;
	};

	struct RenderCompositeMeshDataWithPassConstants : public RenderData
	{
		//GPU buffer resource containing instance data. This is what is actually bound to the pipeline.
		blDataBufferInterfaceResourcePtr constantBuffer;//TEMP DURING RENDER COMPONENT TRANSITION
		blVertexBufferWithPassBufferResourcePtr constantBufferWithPass;
		blDataElementBufferResourcePtr passElementBufferResourcePtr;
		blCompositeMeshResourcePtr compositeMesh;
		blMaterialResourcePtr material;
		blResourceId submeshId;
		size_t instance;
	};
}