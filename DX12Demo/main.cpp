#include "blDX12.h"
#include "blDx12VertexData.h"
#include <memory>

using namespace BoulderLeaf::Graphics::DX12;

int main()
{	
	std::shared_ptr<DX12> dx12(DX12::Get());
	dx12->Initialize();
	Vertex v;
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
