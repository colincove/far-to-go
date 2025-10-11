#include <blFactory.h>
#include "dxgi1_3.h"

namespace BoulderLeaf::Graphics::DX12
{
	blFactory::blFactory()
	{
		DX12_API_CALL(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mFactory)));
	}

	ComPtr<IDXGIFactory2> blFactory::GetDX12Factory()
	{
		return mFactory;
	}
}