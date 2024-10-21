#pragma once

#include <gmock/gmock.h>
#include <directx/d3dx12_core.h>

namespace BoulderLeaf::Graphics::DX12::Test
{
	class MockID3D12GraphicsCommandList :ID3D12GraphicsCommandList
	{
	public:
		MOCK_METHOD(void, ResourceBarrier, (
			_In_  UINT NumBarriers,
			_In_reads_(NumBarriers)  const D3D12_RESOURCE_BARRIER* pBarriers
			), ());
	};
}