#pragma once

/*after adding the include #include <windef.h> I started to get this compile error
#maybe because this is a library.And it does not know what architecture to compile in ?

#C : \Program Files(x86)\Windows Kits\10\Include\10.0.26100.0\um\winnt.h
#define PROBE_ALIGNMENT( _s ) TYPE_ALIGNMENT( DWORD )
#elif !defined(RC_INVOKED)
#error "No Target Architecture"
#endif

#answer
#include <windows.h> */
#include <windows.h>

namespace BoulderLeaf::Graphics
{
	class API
	{
	public:
		virtual void Initialize(HWND hMainWnd) = 0;
	public:
		void RecieveWindowMessage(MSG msg);
	protected:
		virtual void OnWindowMessage(MSG msg) = 0;
		virtual void OnResize() = 0;
	};
}