#pragma once

#include <blTime.h>
#include <windows.h> 
#include <windef.h>
#include <functional>

namespace BoulderLeaf::Core
{
	struct Callbacks
	{
		std::function<void(MSG&)> MessageRecieved;
		std::function<void(const Metrics::blTime&)> Tick;
		std::function<bool()> ShouldContinue;
	};

	void RunGameLoop(Callbacks& callbacks);

	/* Deprecated */
	class blGameLoop
	{
	public:
		using Callbacks = Callbacks;
		blGameLoop();
		void Run(Callbacks& callbacks);
	};
}