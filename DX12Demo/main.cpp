#include<array>
#include "blDX12_LegacyV1.h"
#include "blWindow.h"
#include "Examples.h"
#include <functional>
#include <memory>
#include <windows.h> 
#include <windef.h>
#include <blGameLoop.h>
#include "..\Metrics\blPIX.h"

using namespace BoulderLeaf::Graphics::DX12;
using namespace BoulderLeaf;

int main()
{	
	Metrics::LoadPIX();
	std::shared_ptr<AbstractExample> currentExample;

	BoulderLeaf::Core::blWindow window("DX12 Demo");
	window.SetCallback([&](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT
		{
			if (currentExample.get() != nullptr)
			{
				return currentExample->WndProc(hWnd, msg, wParam, lParam);
			}

			return true;
		});
	
	std::shared_ptr<DX12_LegacyV1> dx12(DX12_LegacyV1::Get());
	DX12_LegacyV1& dx12Ref = *dx12;
	dx12->Initialize(window.GetWindowHandle());

	MSG msg = { };

	Example currentExampleType = Example::None;
	Example nextExampleType = Example::Box;

	Core::blGameLoop gameLoop;
	Core::blGameLoop::Callbacks callbacks =
	{
		.MessageRecieved = [&msg, &dx12](MSG& msgRecieved) -> void
		{
			msg = msgRecieved;
			dx12->RecieveWindowMessage(msg);
		},
		.Tick = [&currentExample, &currentExampleType, &nextExampleType, &dx12](const Metrics::blTime& gameTime) -> void
		{
			if (currentExampleType != nextExampleType)
			{
				currentExample.reset(GetNewExample(dx12, nextExampleType));
				currentExampleType = nextExampleType;
			}

			if (currentExample.get() != nullptr)
			{
				currentExample->Update(gameTime);
			}

			if (currentExample.get() != nullptr)
			{
				currentExample->Draw();
			}
		},
		.ShouldContinue = [&msg]() -> bool
		{
			return msg.message != WM_QUIT;
		},
	};

	gameLoop.Run(callbacks);

	return 1;
}