add_library(
	Math 
	Vector3.cpp)

target_include_directories(Math
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )


install(TARGETS Math CONFIGURATIONS Debug DESTINATION Debug/lib)