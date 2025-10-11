#include <assert.h>
#include "AbstractExample.h"
#include <blDX12_LegacyV1.h>

namespace BoulderLeaf::Graphics::DX12
{
	AbstractExample::AbstractExample(std::shared_ptr<DX12_LegacyV1> dx12) : m_dx12(dx12)
	{
	}

	LRESULT WINAPI AbstractExample::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return true;
	}
}