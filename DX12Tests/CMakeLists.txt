add_executable(
	DX12Tests 
	DX12Tests.cpp
	main.cpp
	MockID3D12Device.h
	MockID3D12GraphicsCommandList.h)

target_include_directories(
	DX12Tests PRIVATE ../DX12
	./
)

target_link_libraries(
	DX12Tests
	PUBLIC DX12
	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
	DirectX-Headers 
)

install(TARGETS DX12Tests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
	TARGET			DX12Tests
    TEST_SUFFIX		.noArgs
    TEST_LIST		noArgsTests)

add_test(NAME DX12Tests COMMAND DX12Tests CONFIGURATIONS Debug)