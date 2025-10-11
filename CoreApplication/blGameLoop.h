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

			std::function<void(Metrics::blTime&)> PreUpdate;
			std::function<void(Metrics::blTime&)> Update;
			std::function<void(Metrics::blTime&)> PostUpdate;

			std::function<void(Metrics::blTime&)> PreDraw;
			std::function<void(Metrics::blTime&)> Draw;
			std::function<void(Metrics::blTime&)> PostDraw;

			std::function<bool()> ShouldContinue;
		};

		blGameLoop();
		void Run(Callbacks& callbacks);
	};
}