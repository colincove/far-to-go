#include <windows.h> 
#include <windef.h>
#include "..\Metrics\blPIX.h"
#include "imgui.h"
#include <blWindow.h>
#include <memory>
#include <blDX12.h>
#include <blGameLoop.h>
#include <blBoxDemoScene.h>
#include <blShapesDemoScene.h>
#include <GeosphereDemoScene.h>
#include <CylinderDemoScene.h>
#include <ShapesWithPassDemoScene.h>
#include <DemoImgui.h>

using namespace BoulderLeaf;
using namespace BoulderLeaf::Graphics;

int main()
{	
	Metrics::LoadPIX();
	std::shared_ptr<Core::blWindow> window(std::make_shared<Core::blWindow>("Graphics Demo"));
	std::shared_ptr<Graphics::API> api(std::make_shared<DX12::blDX12>(window));
	api->Initialize();

	window->SetCallback([&](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
		{
			api->WndProc(hWnd, msg, wParam, lParam);
			return true;
		});

	MSG msg = { };

	std::array<std::shared_ptr<blDemoScene>, 5> demoScenes =
	{
		std::make_shared<blBoxScene>(api, window),
		std::make_shared<blCylinderDemoScene>(api, window),
		std::make_shared<blGeosphereDemoScene>(api, window),
		std::make_shared<blShapesDemoScene>(api, window),
		std::make_shared<ShapesWithPassDemoScene>(api, window),
	};

	Imgui::SetCurrentSelection(1);

	Core::blGameLoop gameLoop;
	Core::blGameLoop::Callbacks callbacks =
	{
		.MessageRecieved = [&msg, &api](MSG& msgRecieved) -> void
		{
			msg = msgRecieved;
			api->RecieveWindowMessage(msg);
		},
		.Tick = [&demoScenes, &api](const Metrics::blTime& gameTime) -> void
		{
			api->Update(gameTime);

			for (const std::shared_ptr<blDemoScene>& demoScenePtr : demoScenes)
			{
				demoScenePtr->Update(gameTime);
			}

			api->StartFrame();
			demoScenes[Imgui::GetCurrentSelection()]->Draw();
			Imgui::DrawDemoImgui();
			api->EndFrame();
		},
		.ShouldContinue = [&msg]() -> bool
		{
			return msg.message != WM_QUIT;
		},
	};

	gameLoop.Run(callbacks);

	return 1;
}