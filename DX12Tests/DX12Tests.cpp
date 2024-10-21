#include "blDX12Test.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "blDX12.h"
#include <directx/d3dx12_core.h>
#include "../DX12/blDx12VertexBuffer.h"
#include "MockID3D12Device.h"
#include "MockID3D12GraphicsCommandList.h"

namespace BoulderLeaf::Graphics::DX12::Test
{
	TEST(DX12, Test)
	{
		EXPECT_TRUE(true);
	}

	TEST(DX12, Initialize)
	{
		std::shared_ptr<DX12> dx12(DX12::Get());
		dx12->Initialize();
		EXPECT_TRUE(dx12->IsInitialized());
	}

	TEST(DX12, CreateDefaultBuffer)
	{
		std::shared_ptr<DX12> dx12(DX12::Get());
		dx12->Initialize();

		MockID3D12Device device = MockID3D12Device();
		//MockID3D12GraphicsCommandList commandList = MockID3D12GraphicsCommandList();

		int initData[] = {
			3, 4, 6
		};
		const void* initDataPtr = static_cast<const void*>(&initData);
		const UINT64 vbByteSize = 8 * sizeof(int);
		ComPtr<ID3D12Resource> uploadBuffer;
		//EXPECT_TRUE(TestFunct(&device, &commandList));
		//ComPtr<ID3D12Resource> resource = CreateDefaultBuffer<MockDevice, MockCommandList>(&device, &commandList, initDataPtr, vbByteSize, uploadBuffer);
		ComPtr<ID3D12Resource> resource = CreateDefaultBuffer(&device, dx12->mCommandList.Get(), initDataPtr, vbByteSize, uploadBuffer);
	}
}