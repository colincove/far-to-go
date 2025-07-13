#pragma once

#include <windows.h> 
#include <windef.h>
#include <string>
#include <functional>

namespace BoulderLeaf::Graphics
{
	class blWindow
	{
	protected:
		static const char* WindowClassName;
	public:
		blWindow(std::string windowName);
		~blWindow();
	private:
		HWND mWindowHandle;
		std::function<LRESULT WINAPI(HWND, UINT, WPARAM, LPARAM)> mCallback;
	public:
		HWND& GetWindowHandle() { return mWindowHandle; }
		LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}