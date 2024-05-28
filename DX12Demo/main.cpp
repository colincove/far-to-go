#include "blDX12.h"
#include <memory>

using namespace BoulderLeaf::Graphics::DX12;

int main()
{	
	std::shared_ptr<DX12> dx12(DX12::Get());
	dx12->Initialize();

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == 275)
		{
			//PostQuitMessage(0);
		}
	}
	
	return 1;
}
