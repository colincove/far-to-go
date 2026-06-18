#include <windows.h> 
#include <windef.h>
#include "..\Metrics\blPIX.h"
#include "imgui.h"
#include <blWindow.h>
#include <memory>
#include <blDX12.h>
#include <blGameLoop.h>
#include <DemoScene01.h>
#include <DemoScene02.h>

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
			return true;
		});

	MSG msg = { };

	blDemoScene01 demoScene(api, window);

	Core::blGameLoop gameLoop;
	Core::blGameLoop::Callbacks callbacks =
	{
		.MessageRecieved = [&msg, &api](MSG& msgRecieved) -> void
		{
			msg = msgRecieved;
			api->RecieveWindowMessage(msg);
		},
		.Tick = [&demoScene, &api](const Metrics::blTime& gameTime) -> void
		{
			api->Update(gameTime);
			demoScene.Update(gameTime);
			api->StartFrame();
			demoScene.Draw();
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