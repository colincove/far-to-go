add_subdirectory(lua)

add_library(
	Scripting
	scripting.cpp)

target_include_directories(Scripting
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )


install(TARGETS Scripting CONFIGURATIONS Debug DESTINATION Debug/lib)

target_link_libraries(
	Scripting
	Lua)