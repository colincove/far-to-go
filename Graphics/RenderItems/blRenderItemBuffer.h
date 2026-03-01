#pragma once

#include <RenderItems/blRenderItemMesh.h>
#include <variant>
#include <memory>
#include <vector>

namespace BoulderLeaf::Graphics
{
	using RenderItemVariant = std::variant<RenderMeshData, RenderMeshDataInstanced>;
	using RenderItemBuffer = std::vector<RenderItemVariant>;
	using RenderItemBufferPtr = std::shared_ptr<RenderItemBuffer>;
}