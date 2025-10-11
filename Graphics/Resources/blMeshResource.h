#pragma once

#include <blResource.h>
#include <blResourceManager.h>
#include <blMesh.h>

namespace BoulderLeaf::Graphics
{
	class blMeshResource : public blResource<blMeshStorage>
	{
	public:
		blMeshResource() = default;
	};
}