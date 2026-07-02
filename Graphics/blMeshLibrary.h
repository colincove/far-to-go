#pragma once

#include <blMesh.h>

namespace BoulderLeaf::Graphics
{
	const StandardMesh& Cube();

	std::unique_ptr<byte[]> CubeInline();

	StandardMesh CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount);
	StandardMesh CreateGeosphere(float radius, unsigned int numSubdivisions);
}