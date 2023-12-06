add_executable(
	DX12Tests 
	DX12Tests.cpp
	main.cpp)

target_link_libraries(
	DX12Tests
	PUBLIC DX12
	GTest::gtest
	GTest::gtest_main
)

install(TARGETS DX12Tests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
	TARGET			DX12Tests
    TEST_SUFFIX		.noArgs
    TEST_LIST		noArgsTests)

add_test(NAME DX12Tests COMMAND DX12Tests CONFIGURATIONS Debug)