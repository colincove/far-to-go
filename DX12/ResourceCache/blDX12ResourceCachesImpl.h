#pragma once

#include <ResourceCache/blDX12ResourceCacheTemplate.h>
#include <Resources/blResources.h>
#include <blUploadBuffer.h>
#include <blDX12Shader.h>

namespace BoulderLeaf::Graphics
{
	struct blShaderResource;
	struct blIndexedMeshResource;
	struct blCompositeMeshResource;

	namespace DX12
	{
		class blConstantBufferDescriptorHeap;
		struct blDX12Mesh;
		struct SubmeshGeometry;
		struct blDX12UploadBuffer;
		class blDX12ListResourceUploadBuffer;
		class blPSO;
		class blCommandList;
		class blDevice;
	}
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ResourceCacheData
	{

	};

	struct blDX12MeshUploadBufferCacheData : public blDX12ResourceCacheData
	{
		blDX12blDataBufferUploadBuffer_Resource translatedMesh;
	};
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ArrayBufferTranslationCacheData : public blDX12ResourceCacheData
	{
		blResourceHandleOfType<blArrayBufferResource> translatedArrayBuffer;
	};

	class blDX12ArrayBufferTranslationCache : public blDX12ResourceCacheTemplate<blArrayBufferResource, blDX12ArrayBufferTranslationCacheData>
	{
	public:
		blDX12ArrayBufferTranslationCache();
	protected:
		virtual void InitializeCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
			blDX12ArrayBufferTranslationCacheData& cache) override;
		virtual void UpdateCacheTemplate(const blResourceHandleOfType<blArrayBufferResource> resource,
			blDX12ArrayBufferTranslationCacheData& cache) override;
	};
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12UploadBufferCacheData : public blDX12ResourceCacheData
	{
		std::unique_ptr<blDX12UploadBuffer> mUploadBuffer;
	};

	class blDX12UploadBufferCache : public blDX12ResourceCacheTemplate<blListResource, blDX12UploadBufferCacheData>
	{
	private:
		blDevice* mDevice;
		blCommandList* mCommandList;
	public:
		blDX12UploadBufferCache(blDevice* device, blCommandList* commandList);
	protected:
		virtual void InitializeCacheTemplate(const blResourceHandleOfType<blListResource> resource,
			blDX12UploadBufferCacheData& cache) override;
	};
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12ShaderCacheData : public blDX12ResourceCacheData
	{
		blDX12ShaderData shaderData;
		std::unique_ptr<blPSO> shaderPSO;
	};

	class blDX12ShaderCache : public blDX12ResourceCacheTemplate<blShaderResource, blDX12ShaderCacheData>
	{
	private:
		blDevice* mDevice;
	public:
		blDX12ShaderCache(blDevice* device);
	protected:
		virtual void InitializeCacheTemplate(
			const blResourceHandleOfType<blShaderResource> resource,
			blDX12ShaderCacheData& cache) override;
	};
}

//NOTE: The name of this cahce is likely bad. Need to understand how it is distinct from my existing upload buffer
//that is used for mesh data being uploaded to the command list for rendering. 
namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12MappedUploadBufferCacheData : public blDX12ResourceCacheData
	{
		std::unique_ptr<blDX12ListResourceUploadBuffer> mUploadBuffer;
	};

	class blDX12MappedUploadBufferCache : public blDX12ResourceCacheTemplate<blListResource, blDX12MappedUploadBufferCacheData>
	{
	private:
		blDevice* mDevice;
	public:
		blDX12MappedUploadBufferCache(blDevice* device);
	protected:
		virtual void InitializeCacheTemplate(const blResourceHandleOfType<blListResource> resource,
			blDX12MappedUploadBufferCacheData& cache) override;
		virtual void UpdateCacheTemplate(const blResourceHandleOfType<blListResource> resource,
			blDX12MappedUploadBufferCacheData& cache) override;
	};
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12DescriptorHeapCacheData : public blDX12ResourceCacheData
	{
		std::unique_ptr<blConstantBufferDescriptorHeap> descriptorHeap;
	};

	class blDX12DescriptorHeapCache : public blDX12ResourceCacheTemplate<blListResource, blDX12DescriptorHeapCacheData>
	{
	private:
		blDevice* mDevice;
		blDX12MappedUploadBufferCache* mMappedUploadBufferCache;
	public:
		blDX12DescriptorHeapCache(blDevice* device, blDX12MappedUploadBufferCache* mappedUploadBufferCache);
	protected:
		virtual void InitializeCacheTemplate(const blResourceHandleOfType<blListResource> resource,
			blDX12DescriptorHeapCacheData& cache) override;
	};
}

namespace BoulderLeaf::Graphics::DX12
{
	struct blDX12CompositeMeshGraftingCacheData : public blDX12ResourceCacheData
	{
		blResourceHandleOfType<blIndexedMeshResource> graftedMesh;
	};

	class blDX12CompositeMeshGraftingCache : public blDX12ResourceCacheTemplate<blCompositeMeshResource, blDX12CompositeMeshGraftingCacheData>
	{
	public:
		blDX12CompositeMeshGraftingCache();
	protected:
		virtual void InitializeCacheTemplate(const blResourceHandleOfType<blCompositeMeshResource> resource,
			blDX12CompositeMeshGraftingCacheData& cache) override;
	};
}