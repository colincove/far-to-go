add_executable(
	GraphicsTests 
	GraphicsTests.cpp
	VirtualCamera.cpp
	main.cpp
	Mesh.cpp)

target_link_libraries(
	GraphicsTests
	PUBLIC Graphics
	GTest::gtest
	GTest::gtest_main
)

install(TARGETS GraphicsTests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
	TARGET			GraphicsTests
   	TEST_SUFFIX		.noArgs
   	TEST_LIST		noArgsTests)

add_test(NAME GraphicsTests COMMAND GraphicsTests CONFIGURATIONS Debug)