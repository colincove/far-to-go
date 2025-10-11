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
#include <blWindow.h>
#include <memory>

namespace BoulderLeaf::Graphics
{
	class API
	{
	private:
		std::shared_ptr<Core::blWindow> mWindow;
	public:
		API() = default;
		API(std::shared_ptr<Core::blWindow> window);
	public:
		void RecieveWindowMessage(MSG msg);
	protected:
		virtual void OnWindowMessage(MSG msg) = 0;
		virtual void OnResize() = 0;
	};
}