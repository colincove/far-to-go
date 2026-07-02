#pragma once

#include <ResourceCache/blDX12ResourceCacheTemplate.h>
#include <ResourceCache/blDX12ResourceCacheData.h>
#include <Resources/blResourcesExprimental.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12MeshTranslationCache : public blDX12ResourceCacheTemplate<blInlineMesh, blDX12MeshTranslationCacheData>
	{
	public:
		blDX12MeshTranslationCache();
	protected:
		virtual void InitializeCacheTemplate(const blResourceHandleOfType<blInlineMesh> resource,
			blDX12MeshTranslationCacheData& cache) override;
	};
}