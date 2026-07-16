#pragma once

#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>

namespace BoulderLeaf
{
	class blResourceContainer;

	namespace Graphics
	{
		struct blInlineMesh;
		struct blCompositeMeshResource;

		namespace DX12
		{
			class blCommandList;
			struct blGlobalRenderData;
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
		blDX12ArrayBufferTranslationCache blArrayBufferTranslationCache;
		blDX12ShaderCache mShaderCache;
		blDX12DescriptorHeapCache mDescriptorHeapCache;
		blDX12MappedUploadBufferCache mMappedUploadBufferCache;
		blDX12CompositeMeshGraftingCache mCompositeMeshGraftingCache;
	public:
		blDX12ResourceCacheGlobalInterface(blGlobalRenderData* globalRenderData);

		const blDX12ArrayBufferTranslationCacheData& GetArrayBufferTranslationCacheData(const blResourceHandleOfType<blArrayBufferResource> resource);
		const blDX12ShaderCacheData& GetShaderCacheData(const blResourceHandleOfType<blShaderResource> resource);
		const blDX12DescriptorHeapCacheData& GetDescriptorHeapCacheData(const blResourceHandleOfType<blListResource> resource);
		const blDX12MappedUploadBufferCacheData& GetMappedUploadBufferCache(const blResourceHandleOfType<blListResource> resource);
		const blDX12CompositeMeshGraftingCacheData& GetCompositeMeshGraftingCache(const blResourceHandleOfType<blCompositeMeshResource> resource);
	};

	class blDX12ResourceCacheCommandListInterface : public blDX12ResourceCacheInterface
	{
	private:
		blCommandList* mCommandList;
	public:
		blDX12ResourceCacheCommandListInterface(blCommandList* commandList);
	};
}