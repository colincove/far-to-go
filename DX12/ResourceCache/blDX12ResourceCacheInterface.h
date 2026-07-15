#pragma once

#include <ResourceCache/blDX12ResourceCachesImpl.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>

namespace BoulderLeaf
{
	class blResourceContainer;

	namespace Graphics
	{
		struct blInlineMesh;

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
	public:
		blDX12ResourceCacheGlobalInterface(blGlobalRenderData* globalRenderData);

		const blDX12ArrayBufferTranslationCacheData& GetArrayBufferTranslationCacheData(const blResourceHandleOfType<blArrayBufferResource> resource);
		const blDX12ShaderCacheData& GetShaderCacheData(const blResourceHandleOfType<blShaderResource_exp> resource);
		const blDX12DescriptorHeapCacheData& GetDescriptorHeapCacheData(const blResourceHandleOfType<blListResource> resource);
		const blDX12MappedUploadBufferCacheData& GetMappedUploadBufferCache(const blResourceHandleOfType<blListResource> resource);
	};

	class blDX12ResourceCacheCommandListInterface : public blDX12ResourceCacheInterface
	{
	private:
		blCommandList* mCommandList;
	public:
		blDX12ResourceCacheCommandListInterface(blCommandList* commandList);
	};
}