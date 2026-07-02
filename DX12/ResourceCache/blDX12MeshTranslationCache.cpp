#include <ResourceCache/blDX12MeshTranslationCache.h>
#include <Resources/blResourcesExprimental.h>

namespace
{
	struct IndexEntry
	{

	};
}

namespace BoulderLeaf::Graphics::DX12
{
	blDX12MeshTranslationCache::blDX12MeshTranslationCache()
		: blDX12ResourceCacheTemplate()
	{

	}

	void blDX12MeshTranslationCache::InitializeCacheTemplate(const blResourceHandleOfType<blInlineMesh> resource,
		blDX12MeshTranslationCacheData& cache)
	{
		blResourceContainer* container = resource.GetContainer();

	}
}