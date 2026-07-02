#include <DemoScene.h>

namespace BoulderLeaf::Graphics
{
	blDemoScene::blDemoScene(std::shared_ptr<API> graphicsAPI, std::shared_ptr<Core::blWindow> window, blResourceContainer* resourceContainer)
		: mGraphicsAPI(graphicsAPI), mWindow(window), mResourceContainer(resourceContainer)
	{

	}
}