#pragma once
#include <blTypes.h>
#include <blRenderScene.h>
#include <blRenderDataTypes.h>

namespace BoulderLeaf::Graphics
{
	namespace
	{
		template<typename TData, RenderDataType type>
		void WriteRenderData(blRenderScene& scene, const TData& data)
		{
			scene.WriteRenderData<TData, type>(data);
			// This function would write the data to the render item buffer in the scene.
			// The actual implementation would depend on how the render item buffer is structured.
			// For now, we will just leave it empty as a placeholder.
		}
	}

	struct MeshRenderData
	{

	};

	using WriteMeshRenderData = WriteRenderData<MeshRenderData, RenderDataType::Mesh>
}