add_executable(
	MetricsTests
	main.cpp
	Time.cpp
	)

target_link_libraries(
	MetricsTests 
	PUBLIC Metrics
	GTest::gtest
	GTest::gtest_main
)

install(TARGETS MetricsTests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
    TARGET		MetricsTests
    TEST_LIST		noArgsTests)