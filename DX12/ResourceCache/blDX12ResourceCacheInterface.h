#pragma once

#include <ResourceCache/blDX12ResourceCacheData.h>
#include <ResourceCache/blDX12MeshTranslationCache.h>

namespace BoulderLeaf
{
	class blResourceContainer;

	namespace Graphics
	{
		struct blInlineMesh;

		namespace DX12
		{
			class blCommandList;
		}
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12ResourceCacheInterface
	{
	public:
		blDX12ResourceCacheInterface();
	};

	class blDX12ResourceCacheGlobalInterface : public blDX12ResourceCacheInterface
	{
	private:
		blDX12MeshTranslationCache mMeshTranslationCache;
	public:
		blDX12ResourceCacheGlobalInterface();

		const blDX12MeshTranslationCacheData& GetMeshTranslationCacheData(const blResourceHandleOfType<blInlineMesh> resource);
	};

	class blDX12ResourceCacheCommandListInterface : public blDX12ResourceCacheInterface
	{
	private:
		blCommandList* mCommandList;
	public:
		blDX12ResourceCacheCommandListInterface(blCommandList* commandList);
	};
}