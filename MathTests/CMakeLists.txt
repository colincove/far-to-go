add_executable(
	MathTests
	Line.cpp
	LineSegment.cpp
	main.cpp
	Math.cpp
	Matrix2x2.cpp
	Matrix2x3.cpp
	Matrix3x3.cpp
	Matrix4x4.cpp
	Point3.cpp
	Quad.cpp
	Quaternion.cpp
	Ray.cpp
	Rotation.cpp
	Rect.cpp
	Transformations.cpp
	Vector2.cpp
	Vector3.cpp
	)

target_link_libraries(
	MathTests 
	PUBLIC Math
	GTest::gtest
	GTest::gtest_main
)

install(TARGETS MathTests CONFIGURATIONS Debug DESTINATION Debug)

gtest_add_tests(
    TARGET		MathTests
    TEST_LIST		noArgsTests)

#add_test(NAME MathTests COMMAND MathTests CONFIGURATIONS Debug)