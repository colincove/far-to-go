#include<array>
#include "blDX12.h"
#include "Examples.h"
#include <functional>
#include <memory>
#include <windows.h> 
#include <windef.h>
#include "..\Metrics\blPIX.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

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

	WNDCLASSEX wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, "DX12 Demo", nullptr };
	::RegisterClassEx(&wc);
	HWND mhMainWnd = ::CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, "DX12 Demo", WS_VISIBLE | WS_THICKFRAME | WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, NULL, NULL, NULL, NULL);
	
	std::shared_ptr<DX12> dx12(DX12::Get());
	DX12& dx12Ref = *dx12;
	dx12->Initialize(mhMainWnd);
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
