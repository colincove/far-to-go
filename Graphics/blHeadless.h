#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	class Headless : public API
	{

	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	public:
		virtual void DrawMesh(const RenderMeshData& renderData) override;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData) override;
		virtual void DrawMeshInstancedV2(const RenderMeshDataInstancedV2& renderData) override;
		virtual void DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData) override;
		virtual void DrawCompositeMeshWithPass(const RenderCompositeMeshDataWithPassConstants& renderData) override;
	};
}