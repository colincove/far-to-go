#pragma once

#include <memory>
#include <blGraphicsAPI.h>
#include <blTime.h>
#include <blWindow.h>

namespace BoulderLeaf::Graphics
{
	class blDemoScene
	{
	protected:
		std::shared_ptr<API> mGraphicsAPI;
		std::shared_ptr<Core::blWindow> mWindow;
	public:
		blDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window);
		virtual void Draw() = 0;
		virtual void Update(const Metrics::blTime& gameTime) = 0;
	};
}