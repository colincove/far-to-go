add_library(
	Math 
	Vector3.h
	Vector3.cpp
	Matrix4x4.h
	Matrix4x4.cpp
	BLMath.h
	BLMath.inl)

target_include_directories(Math
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )


install(TARGETS Math CONFIGURATIONS Debug DESTINATION Debug/lib)