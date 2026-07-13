#include "blGameLoop.h"
#include <assert.h>


namespace BoulderLeaf::Core
{
	void RunGameLoop(Callbacks& callbacks)
	{
		MSG msg = { };
		Metrics::blTime gameTime;
		bool isFirstTick = true;

		assert(callbacks.ShouldContinue && "Must provide a valid ShouldContinue callback");
		while (callbacks.ShouldContinue())
		{
			if (isFirstTick)
			{
				isFirstTick = false;
			}
			else
			{
				gameTime = gameTime.NewTick();
			}

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (callbacks.MessageRecieved)
				{
					callbacks.MessageRecieved(msg);
				}
			}

			if (callbacks.Tick)
			{
				callbacks.Tick(gameTime);
			}
		}
	}

	blGameLoop::blGameLoop()
	{

	}

	void blGameLoop::Run(Callbacks& callbacks)
	{
		RunGameLoop(callbacks);
	}
}