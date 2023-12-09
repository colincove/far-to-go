#include "blDX12.h"
#include <memory>

using namespace BoulderLeaf::Graphics::DX12;

int main()
{	
	std::shared_ptr<DX12> dx12(DX12::Get());
	dx12->Initialize();
	return 1;
}
