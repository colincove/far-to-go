#include<array>
#include "blDX12_LegacyV1.h"
#include "blWindow.h"
#include "Examples.h"
#include <functional>
#include <memory>
#include <windows.h> 
#include <windef.h>
#include "..\Metrics\blPIX.h"

using namespace BoulderLeaf::Graphics::DX12;
using namespace BoulderLeaf;

namespace
{
	std::shared_ptr<AbstractExample> currentExample;
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

int main()
{	
	Metrics::LoadPIX();

	BoulderLeaf::Graphics::blWindow window("DX12 Demo");
	window.SetCallback(WndProc);
	
	std::shared_ptr<DX12_LegacyV1> dx12(DX12_LegacyV1::Get());
	DX12_LegacyV1& dx12Ref = *dx12;
	dx12->Initialize(window.GetWindowHandle());

	MSG msg = { };

	Example currentExampleType = Example::None;
	Example nextExampleType = Example::ImGui;

	Metrics::blTime gameTime;

	while (true)
	{
		gameTime = gameTime.NewTick();

		if (currentExampleType != nextExampleType)
		{
			currentExample.reset(GetNewExample(dx12, nextExampleType));
			currentExampleType = nextExampleType;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			dx12->RecieveWindowMessage(msg);
		}

		currentExample->Update(gameTime);
		currentExample->Draw();	
		::Sleep(10);
	}
	
	return 1;
}

namespace
{
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (currentExample.get() != nullptr)
		{
			return currentExample->WndProc(hWnd, msg, wParam, lParam);
		}

		return true;
	}
}
