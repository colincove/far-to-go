#pragma once

#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	class blGraphicsAPIImpl : public API
	{
	public:
		blGraphicsAPIImpl() = default;
		blGraphicsAPIImpl(Core::blWindow* window) : API(window)
		{
		}
	};
}