#pragma once

#include <Resources/blResourcesExprimental.h>
#include <blMesh.h>
#include <blUploadBuffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ResourceCacheData
	{

	};

	struct blDX12MeshTranslationCacheData : public blDX12ResourceCacheData
	{
		blResourceHandleOfType<blInlineMesh> translatedMesh;
	};

	struct blDX12MeshUploadBufferCacheData : public blDX12ResourceCacheData
	{
		blDX12blDataBufferUploadBuffer_Resource translatedMesh;
	};
}