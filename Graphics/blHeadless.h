#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	class Headless : public API
	{

	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	public:
		virtual void DrawMesh(const RenderMeshData& renderData, const  blSceneResourcePtr scene) override;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene) override;
		virtual void DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData, const blSceneResourcePtr scene) override;
	};
}