#pragma once

#include <blTime.h>
#include <windows.h> 
#include <windef.h>
#include <functional>

namespace BoulderLeaf::Core
{
	class blGameLoop
	{
	public:

		struct Callbacks
		{
			std::function<void(MSG&)> MessageRecieved;
			std::function<void(const Metrics::blTime&)> Tick;
			std::function<bool()> ShouldContinue;
		};

		blGameLoop();
		void Run(Callbacks& callbacks);
	};
}