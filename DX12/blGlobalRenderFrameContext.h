#pragma once

#include <blTime.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12;

	struct blGlobalRenderFrameContext
	{
		friend blDX12;

		const Metrics::blTime& GetCurrentGameTime() const
		{
			return currentGameTime;
		}

		int GetCurrentFrameResource() const
		{
			return currentFrameResource;
		}

	private:
		Metrics::blTime currentGameTime;
		int currentFrameResource;
	};
}