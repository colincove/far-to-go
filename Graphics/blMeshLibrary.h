#pragma once

#include <blMesh.h>

namespace BoulderLeaf::Graphics
{
	const StandardMesh& Cube();

	std::unique_ptr<byte[]> CubeInline();
	blResourceHandleOfType<blBufferDescriptionResource> BufferDescriptionResource(blResourceContainer* container, const BufferDescription& description);
	blResourceHandleOfType<blIndexedMeshResource> CubeResource(blResourceContainer* container);

	StandardMesh CreateCylinder(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount);
	StandardMesh CreateGeosphere(float radius, unsigned int numSubdivisions);

	blResourceHandleOfType<blIndexedMeshResource> CreateCylinderResource(blResourceContainer* container, float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount);
	blResourceHandleOfType<blIndexedMeshResource> CreateGeosphereResource(blResourceContainer* container, float radius, unsigned int numSubdivisions);
}