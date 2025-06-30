#include <blGraphicsAPI.h>

namespace BoulderLeaf::Graphics
{
	void API::RecieveWindowMessage(MSG msg)
	{
		switch (msg.message)
		{
		case WM_EXITSIZEMOVE:
		case CB_SETDROPPEDWIDTH:
			OnResize();
			break;
		}

		OnWindowMessage(msg);
	}

	void API::OnWindowMessage(MSG msg)
	{

	}

	void API::OnResize()
	{
	}
}