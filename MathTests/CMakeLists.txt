add_executable(
	MathTests
	Vector3.cpp
	main.cpp)

target_link_libraries(
	MathTests 
	PUBLIC Math
	GTest::gtest
	GTest::gtest_main
)

install(TARGETS MathTests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
	TARGET			MathTests
    TEST_SUFFIX		.noArgs
    TEST_LIST		noArgsTests)

add_test(NAME MathTests COMMAND MathTests CONFIGURATIONS Debug)