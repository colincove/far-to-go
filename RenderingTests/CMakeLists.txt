add_executable(
	RenderingTests 
	RenderingTests.cpp
	main.cpp)

target_link_libraries(
	RenderingTests
	PUBLIC Rendering
	GTest::gtest
	GTest::gtest_main
)

install(TARGETS RenderingTests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
	TARGET			RenderingTests
    TEST_SUFFIX		.noArgs
    TEST_LIST		noArgsTests)

add_test(NAME RenderingTests COMMAND RenderingTests CONFIGURATIONS Debug)