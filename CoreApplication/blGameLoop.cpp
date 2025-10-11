#include "blGameLoop.h"
#include <assert.h>


namespace BoulderLeaf::Core
{
	blGameLoop::blGameLoop()
	{

	}

	void blGameLoop::Run(Callbacks& callbacks)
	{
		MSG msg = { };
		Metrics::blTime gameTime;

		assert(callbacks.ShouldContinue && "Must provide a valid ShouldContinue callback");
		while (callbacks.ShouldContinue())
		{
			gameTime = gameTime.NewTick();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (callbacks.MessageRecieved)
				{
					callbacks.MessageRecieved(msg);
				}
			}

			if (callbacks.PreUpdate)
			{
				callbacks.PreUpdate(gameTime);
			}

			if (callbacks.Update)
			{
				callbacks.Update(gameTime);
			}

			if (callbacks.PostUpdate)
			{
				callbacks.PostUpdate(gameTime);
			}

			if (callbacks.PreDraw)
			{
				callbacks.PreDraw(gameTime);
			}

			if (callbacks.Draw)
			{
				callbacks.Draw(gameTime);
			}

			if (callbacks.PostDraw)
			{
				callbacks.PostDraw(gameTime);
			}

			::Sleep(10);
		}
	}
}