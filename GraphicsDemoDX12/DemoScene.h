#pragma once

#include <memory>

namespace BoulderLeaf
{
	class blResourceContainer;

	namespace Graphics
	{
		class API;
	}

	namespace Core
	{
		class blWindow;
	}

	namespace Metrics
	{
		class blTime;
	}
}

namespace BoulderLeaf::Graphics
{
	class blDemoScene
	{
	protected:
		API* mGraphicsAPI;
		Core::blWindow* mWindow;
		blResourceContainer* mResourceContainer;
	public:
		blDemoScene(API* graphicsAPI, Core::blWindow* window, blResourceContainer* resourceContainer);
		virtual void Draw() = 0;
		virtual void Update(const Metrics::blTime& gameTime) = 0;
	};
}