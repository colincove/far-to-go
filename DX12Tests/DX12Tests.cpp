#include <blDX12.h>
#include <blDX12Test.h>
#include <blDx12VertexBuffer.h>
#include <directx/d3dx12_core.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MockID3D12Device.h>
#include <MockID3D12GraphicsCommandList.h>

namespace BoulderLeaf::Graphics::DX12::Test
{
	namespace
	{
		UINT64 MockUpdateSubresources(UPDATE_SUBRESOURCES_PARAMETERS)
		{
			return 0;
		}
	}

	TEST(DX12, Initialize)
	{
		std::shared_ptr<DX12> dx12(DX12::Get());
		dx12->Initialize();
		EXPECT_TRUE(dx12->IsInitialized());
	}

	TEST(DX12, CreateDefaultBuffer)
	{
		MockID3D12Device device = MockID3D12Device();
		MockID3D12GraphicsCommandList commandList = MockID3D12GraphicsCommandList();

		using ::testing::Return;

		//these are likely not good expectations to set for these tests. But we want some tests
		EXPECT_CALL(device, CreateCommittedResource)
			.Times(2)
			.WillRepeatedly(Return(S_OK));

		EXPECT_CALL(commandList, ResourceBarrier)
			.Times(2);

		int initData[] = {
			3, 4, 6
		};

		const void* initDataPtr = static_cast<const void*>(&initData);
		const UINT64 vbByteSize = 8 * sizeof(int);

		ComPtr<ID3D12Resource> uploadBuffer;
		ComPtr<ID3D12Resource> resource = CreateDefaultBuffer(&device, &commandList, initDataPtr, vbByteSize, uploadBuffer, MockUpdateSubresources);
	}
}