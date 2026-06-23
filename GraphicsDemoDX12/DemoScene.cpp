#include <DemoScene.h>

namespace BoulderLeaf::Graphics
{
	blDemoScene::blDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window)
		: mGraphicsAPI(graphicsAPI), mWindow(window)
	{

	}
}