#pragma once

#include <blRenderItem.h>
#include <blMesh.h>
#include <memory>

namespace BoulderLeaf::Graphics
{
	struct RenderItemStandardMeshData
	{
		std::shared_ptr<StandardMesh> mesh;
	};

	using RenderItemStandardMesh = RenderItem<RenderItemStandardMeshData, RenderItemType::Mesh>;
}