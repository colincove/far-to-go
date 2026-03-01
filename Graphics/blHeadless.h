#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	class Headless : public API
	{

	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
		virtual void DrawMesh(const RenderMeshData& renderData, const  blSceneResourcePtr scene) override;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene) override;
	};
}