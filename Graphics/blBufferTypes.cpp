#include <blBufferTypes.h>
#include <blMatrix4x4.inl>
#include <blVirtualCamera.h>
#include <algorithm>

namespace BoulderLeaf::Graphics
{
	const std::vector<BufferElementDescription> blStandardObjectConstantsDefinition::Description =
	{
		{ "WorldViewProj", BufferElementType::Matrix4x4, BufferElementSemantic::None }
	};

	cbPass CalculatePassData(const VirtualCamera& camera, const Metrics::blTime& gameTime)
	{
		using namespace Math;

		//TODO: Where can I get this?
		constexpr Vector4 up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		//TODO: Get from camera
		const Matrix4x4 view = camera.GetView();
		bool hasInverse = view.HasValidInverse();
		const Matrix4x4 viewInverted = view.Inverse();
		float determinant = view.Determinant();

		cbPass result;
		result.Proj = camera.GetProjectionMatrix();
		result.invProj = result.Proj.Inverse();
		result.view = camera.GetView();

		Vector4 target = Vector4::Zero();
		Vector4 pos = Vector4(-0.660871327, 14.8207569, -13.4129944, 1.0f);
		const Matrix4x4 lookAtMatrix = Matrix4x4::LookAtRH(pos, target, up); // Camera to world
		const Matrix4x4 lookAtMatrixInverse = lookAtMatrix.Inverse();
		const Matrix4x4 inlineView = Matrix4x4(lookAtMatrixInverse.r0, lookAtMatrixInverse.r1, lookAtMatrixInverse.r2, lookAtMatrix.r3);
		const Matrix4x4 inlineViewDirect = Matrix4x4::ViewMatrix(pos, target, up);
		//result.view = Matrix4x4::ViewMatrix(pos, target, up);
		//result.view = lookAtMatrixInverse;
		result.view = inlineViewDirect;

		result.invView = result.view.Inverse();
		result.viewProj =  result.view * result.Proj;
		result.invViewProj = result.viewProj.Inverse();

		return result;
	}

	void CalculateHeightBoundsForTerrain(const blTerrainVertex* data, uint32_t count, float& minOut, float& maxOut)
	{
		minOut = 0;
		maxOut = 0;
		for (const blTerrainVertex* vertex = data; vertex != data + count; ++vertex)
		{
			//(std::min) to prevent macro expansion. There is a winows macro defined, 'min'. 
			//there are other solutions. This is what I am using, right now. 
			minOut = (std::min)(minOut, vertex->Height);
			maxOut = (std::max)(maxOut, vertex->Height);
		}
	}
}