#include <gtest/gtest.h>
#include "blDX12Test.h"
//Standard
#include <iostream>

int main()
{
	BoulderLeaf::Graphics::DX12::Test::LogDevices();
	std::cout << "\n";
	testing::InitGoogleTest();
	RUN_ALL_TESTS();
}