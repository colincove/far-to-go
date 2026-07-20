#include <blBufferTypes.h>
#include <blMatrix4x4.inl>
#include <blVirtualCamera.h>
#include <algorithm>

namespace BoulderLeaf::Graphics
{
	const std::vector<BufferElementDescription> blStandardObjectConstantsDefinition::Description =
	{
		{ "WorldViewProj", BufferElementType::Matrix4x4 }
	};

	cbPass CalculatePassData(const VirtualCamera& camera, const Metrics::blTime& gameTime)
	{
		using namespace Math;

		//TODO: Where can I get this?
		constexpr Vector4 up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);

		//TODO: Get from camera
		const static Matrix4x4 cameraTranslate = Matrix4x4::TranslationMatrix(-2, 0, 3);
		const static Matrix4x4 world = Matrix4x4::TranslationMatrix(Vector3());

		cbPass result;
		result.Proj = camera.GetProjectionMatrix();
		result.viewProj = (world * camera.GetProjectionMatrix());

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