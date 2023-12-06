#include "blDX12Test.h"
#include <gtest/gtest.h>

namespace BoulderLeaf::Graphics::DX12::Test
{
	TEST(DX12, Test)
	{
		EXPECT_TRUE(true);
	}

	TEST(DX12, CommandQueue)
	{
		CreateCommandList();
	}
}