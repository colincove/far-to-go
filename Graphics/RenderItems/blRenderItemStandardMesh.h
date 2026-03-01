#pragma once

#include <RenderItems/blRenderItem.h>
#include <blMesh.h>
#include <blMaterial.h>
#include <Resources/blResource.h>
#include <blMatrix4x4.inl>
#include <memory>

namespace BoulderLeaf::Graphics
{
	struct RenderItemMeshData
	{
		Matrix4x4 transform;
		blMeshBaseResourceWeakPtr mesh;
		blMaterialResourceWeakPtr material;
	};

	using RenderItemMesh = RenderItem<RenderItemMeshData, RenderItemType::Mesh>;
}