#include <blGraphicsAPI_LegacyV1.h>

namespace BoulderLeaf::Graphics
{
	void API_LegacyV1::RecieveWindowMessage(MSG msg)
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

	void API_LegacyV1::OnWindowMessage(MSG msg)
	{

	}

	void API_LegacyV1::OnResize()
	{
	}
}