#pragma once

#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	struct ResourceState
	{
		bool IsReady;
		bool HasBeenProcessed;
	};

	template<typename TResourceState = ResourceState>
	class blGraphicsAPIImpl : public API
	{
	public:
		blGraphicsAPIImpl() = default;
		blGraphicsAPIImpl(std::shared_ptr<Core::blWindow> window) : API(window)
		{

		}
	private:
		std::vector<TResourceState> mResourceStates;
	protected:
		TResourceState& GetResourceState(const blResourceId& resourceId)
		{
			if (resourceId.GetRaw() >= mResourceStates.size())
			{
				mResourceStates.resize(resourceId.GetRaw() + 1);
			}
			return mResourceStates[resourceId.GetRaw()];
		}
	};
}