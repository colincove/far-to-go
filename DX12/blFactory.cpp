#include <blFactory.h>

namespace BoulderLeaf::Graphics::DX12
{
	blFactory::blFactory()
	{

	}

	ComPtr<IDXGIFactory2> blFactory::GetDX12Factory()
	{
		return mFactory;
	}
}