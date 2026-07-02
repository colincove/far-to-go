#include <ResourceCache/blDX12ResourceCacheInterface.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12ResourceCacheInterface::blDX12ResourceCacheInterface()
	{

	}

	blDX12ResourceCacheGlobalInterface::blDX12ResourceCacheGlobalInterface()
		: blDX12ResourceCacheInterface(),
		mMeshTranslationCache()
	{

	}

	const blDX12MeshTranslationCacheData& blDX12ResourceCacheGlobalInterface::GetMeshTranslationCacheData(const blResourceHandleOfType<blInlineMesh> resource)
	{
		return mMeshTranslationCache.GetTypedCachedData(resource);
	}
}