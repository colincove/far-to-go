add_executable(
	CoreApplicationTests 
	ResourceTests.cpp
	main.cpp
)

target_include_directories(
	CoreApplicationTests PRIVATE
	../CoreApplication
	./
)

target_link_libraries(
	CoreApplicationTests
	PUBLIC CoreApplication
	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
)

# Link CoreApplication so tests can use its implementation (blResources.cpp)
target_link_libraries(
	CoreApplicationTests
	PRIVATE CoreApplication
)

install(TARGETS CoreApplicationTests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
	TARGET			CoreApplicationTests
    TEST_SUFFIX		.noArgs
    TEST_LIST		noArgsTests)

add_test(NAME CoreApplicationTests COMMAND CoreApplicationTests CONFIGURATIONS Debug)