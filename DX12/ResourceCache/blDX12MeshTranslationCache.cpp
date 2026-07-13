#include <ResourceCache/blDX12MeshTranslationCache.h>
#include <Resources/blResourcesExprimental.h>
#include <blDX12Buffer.h>

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
		const blInlineMesh& resourceRef = *resource;

		if (resourceRef.mHeader.mFormat == BufferFormat::DX12)
		{
			//already in the expected format. 
			cache.translatedMesh = resource;
			return;
		}

		if (!resourceRef.mHeader.description)
		{
			//cannot translate a mesh without a description.
			return;
		}

		/*
		auto descriptionResourceHandle = blResourceHandleOfType<InlineBufferDescription>(
			container->CreateHandleFromRef(resourceRef.mHeader.description));

		blInlineMesh::Header translatedHeader;

		{
			translatedHeader = resourceRef.mHeader;
			translatedHeader.mFormat = BufferFormat::DX12;
			translatedHeader.mVertexSize = Math::CalcConstantBufferByteSize(
				GetBufferElementSize(descriptionResourceHandle->ToBufferDescription().elementDescriptions, DX12::DX12BufferAdapter::Get()));

			blInlineMesh translatedMeshTemp;
			translatedMeshTemp.mHeader = translatedHeader;
			cache.translatedMesh = container->CreateResourceOfType<blInlineMesh>(
				std::wstring(resource.GetName()), translatedMeshTemp.GetTotalSize());
		}

		blInlineMesh& translatedMesh = *cache.translatedMesh;
		translatedMesh.mHeader = translatedHeader;
		translatedMesh.mHeader.description = resourceRef.mHeader.description;

		MarshalBufferElement(descriptionResourceHandle->ToBufferDescription().elementDescriptions,
			translatedMesh.GetVertexStartMutable(),
			resourceRef.GetVertexStart(),
			DX12::DX12BufferAdapter::Get());

		memcpy(translatedMesh.GetIndicesStartMutable(),
			resourceRef.GetIndicesStart(),
			resourceRef.mHeader.mIndexCount * sizeof(blMeshStorage::index));*/
	}
}