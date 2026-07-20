#pragma once

#include <blMesh.h>
#include <blBufferTypes.h>

namespace BoulderLeaf::Graphics
{
	blResourceHandleOfType<blBufferDescriptionResource> BufferDescriptionResource(blResourceContainer* container, const BufferDescription& description);
	blResourceHandleOfType<blIndexedMeshResource> CubeResource(blResourceContainer* container);

	blResourceHandleOfType<blIndexedMeshResource> CreateCylinderResource(blResourceContainer* container, float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount);
	blResourceHandleOfType<blIndexedMeshResource> CreateGeosphereResource(blResourceContainer* container, float radius, unsigned int numSubdivisions);
	blResourceHandleOfType<blIndexedMeshResource> CreateTerrainResource(blResourceContainer* container, uint16_t size);
}