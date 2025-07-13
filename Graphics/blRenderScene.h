#pragma once

#include <blVirtualCamera.h>
#include <blTypes.h>
#include <blRenderDataTypes.h>
#include <streambuf>
#include <vector>

namespace BoulderLeaf::Graphics
{
	struct blRenderSceneData
	{
		VirtualCamera m_camera;
	};

	class blRenderScene
	{
		constexpr static size_t RENDER_DATA_BUFFER_SIZE = 1024 * 1024; // 1 MB
	public:
		blRenderScene() = default;
		~blRenderScene() = default;

	public:
		template<typename TData, RenderDataType type>
		void WriteRenderData(const TData& data)
		{
			// Ensure that the stream position does not exceed the buffer size
			if (mStreamPosition + sizeof(TData) > RENDER_DATA_BUFFER_SIZE)
			{
				throw std::runtime_error("Render item buffer overflow");
			}
			// Write data to the render item buffer
			std::memcpy(mRenderItemBuffer + mStreamPosition, &data, sizeof(TData));
			mRenderItemPositions[static_cast<size_t>(type)].push_back(mStreamPosition);
			mStreamPosition += sizeof(TData);
		}
	private:
		blRenderSceneData mData;
		byte mRenderItemBuffer[RENDER_DATA_BUFFER_SIZE];
		size_t mStreamPosition;
		std::vector<size_t> mRenderItemPositions[static_cast<size_t>(RenderDataType::Count)];
	};
}