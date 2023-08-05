add_library(
	Math 
	BLMath.inl
	BLMathFloat.h
	Line.inl
	LineSegment.inl
	Matrix4x4.inl
	Plane.inl
	Quad.inl
	Ray.inl
	Rect.inl
	Vector2.inl
	Vector3.inl
	Vector3.cpp
	)

# Note: I need this because I did not include any *.cpp files to be linked. 
# I had renamed them all to *.inl. 
set_target_properties(Math PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Math
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )


install(TARGETS Math CONFIGURATIONS Debug DESTINATION Debug/lib)