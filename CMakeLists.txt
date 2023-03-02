set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

cmake_minimum_required(VERSION 3.12)

project(
	"Far to Go" 
	VERSION 0.0.1.0)

add_executable(
	FarToGo 
	FarToGo/main.cpp)

enable_testing()

add_subdirectory(Math)
add_subdirectory(MathTests)

target_link_libraries(
	FarToGo 
	PUBLIC Math)

target_include_directories(
	FarToGo 
	PUBLIC "${PROJECT_BINARY_DIR}")

configure_file(
	"${PROJECT_SOURCE_DIR}/FarToGo/Config.h.in" 
	"${PROJECT_SOURCE_DIR}/FarToGo/Config.h")

install(TARGETS FarToGo CONFIGURATIONS Debug DESTINATION Debug)

add_test(NAME Runs COMMAND FarToGo 25)