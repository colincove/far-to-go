#pragma once

#include <windows.h> 
#include <windef.h>
#include <string>
#include <functional>

namespace BoulderLeaf::Core
{
	class blWindow
	{
	protected:
		static const char* WindowClassName;
	public:
		using Callback = std::function<LRESULT WINAPI(HWND, UINT, WPARAM, LPARAM)>;

		blWindow(std::string windowName);
		~blWindow();
	private:
		HWND mWindowHandle;
		Callback mCallback;
	public:
		HWND& GetWindowHandle() { return mWindowHandle; }
		void SetCallback(Callback callback) { mCallback = callback; }
		LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		int GetWidth() const;
		int GetHeight() const;
	};
}