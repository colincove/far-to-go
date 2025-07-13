set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# after adding bullet3 physics, there was a compile error. These errors involved using tuple from the TR1 (Technical Report 1) namespace
# the tuple class was included in cXX11, so we want the standard 'std' namespace version.
# optionally, defining _HAS_TR1_NAMESPACE also resolved the issue, as it includes the TR1 namespace in the compilation. SSS
add_compile_definitions(GTEST_LANG_CXX11=1)

cmake_minimum_required(VERSION 3.12)

project(
	"Far to Go" 
	VERSION 0.0.1.0)

enable_testing()

include(${CMAKE_CURRENT_SOURCE_DIR}/DirectXShaderCompiler/cmake/caches/PredefinedParams.cmake)

#NOTE: We are not fetching GTest here because it conflicts with the libraries imported via bullet3.
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# still need this in order to input package functions such as gtest_add_tests
find_package(GTest)

add_executable(
	FarToGo 
	FarToGo/main.cpp
	Test/blAssert.h)

# fatal error (exceeded object file format limit) and the suggestion to compile with the flag /bigobj
# started happening in DX12Tests.cpp after I finished adding my Mock classes.
add_compile_options(/bigobj)

add_subdirectory(DirectX-Headers)
add_subdirectory(Math)
add_subdirectory(Metrics)
add_subdirectory(MetricsTests)
add_subdirectory(Physics)
add_subdirectory(MathTests)
add_subdirectory(Scripting)
add_subdirectory(DX12)
add_subdirectory(DX12Demo)
add_subdirectory(DX12Tests)
add_subdirectory(Graphics)
add_subdirectory(GraphicsTests)
add_subdirectory(GraphicsDemoDX12)
add_subdirectory(RapidXML)
add_subdirectory(imgui)
add_subdirectory(Core)

target_link_libraries(
	FarToGo 
	PUBLIC Math
	Physics
	Scripting
	Graphics
	RapidXML)

target_include_directories(
	FarToGo 
	PUBLIC "${PROJECT_BINARY_DIR}")

configure_file(
	"${PROJECT_SOURCE_DIR}/FarToGo/Config.h.in" 
	"${PROJECT_SOURCE_DIR}/FarToGo/Config.h")

install(TARGETS FarToGo CONFIGURATIONS Debug DESTINATION Debug)
add_test(NAME Runs COMMAND FarToGo CONFIGURATIONS Debug)