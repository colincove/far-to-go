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
		blStandardObjectConstantsBufferResourcePtr constantBuffer;
		blMeshBaseResourcePtr mesh;
		blMaterialResourcePtr material;
	};
}