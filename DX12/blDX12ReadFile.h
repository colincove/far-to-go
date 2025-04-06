#pragma once

#include "blDX12Core.h"
#include <D3Dcommon.h>
#include <string>

namespace BoulderLeaf::Graphics::DX12
{
	ComPtr<ID3DBlob> LoadBinary(const std::wstring filePath);
}