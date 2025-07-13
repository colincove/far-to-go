#include <blWindow.h>
#include <string>
#include <map>

namespace
{
	using namespace BoulderLeaf::Graphics;
	std::map<HWND, blWindow*> windowMap;
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

namespace BoulderLeaf::Graphics
{
	const char* blWindow::WindowClassName = "BoulderLeaf";

	blWindow::blWindow(std::string windowName):
		mWindowHandle(nullptr)
	{
		WNDCLASSEX wc = { sizeof(wc), CS_CLASSDC, ::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, WindowClassName, nullptr };
		::RegisterClassEx(&wc);
		HWND mhMainWnd = ::CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, windowName.c_str(), WS_VISIBLE | WS_THICKFRAME | WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, NULL, NULL, NULL, NULL);
		windowMap.emplace(mhMainWnd, this);
	}

	blWindow::~blWindow()
	{
		if (mWindowHandle)
		{
			windowMap.erase(mWindowHandle);
			::DestroyWindow(mWindowHandle);
			mWindowHandle = nullptr;
		}

		::UnregisterClass(WindowClassName, GetModuleHandle(nullptr));
	}

	LRESULT WINAPI blWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (mCallback)
		{
			return mCallback(hWnd, msg, wParam, lParam);
		}

		return true;
	}
}

namespace
{
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto it = windowMap.find(hWnd);
		if (it != windowMap.end())
		{
			blWindow* window = it->second;
			if (window)
			{
				return window->WndProc(hWnd, msg, wParam, lParam);
			}
		}
		return true;
	}
}