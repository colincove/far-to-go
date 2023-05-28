add_subdirectory(DirectX-Headers)

add_library(
	Rendering 
	rendering.cpp)

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")
#add_subdirectory(lib)

target_include_directories(Rendering
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(Rendering PRIVATE
	DirectX-Headers DirectX-Guids ${dxlibs})

install(TARGETS Rendering CONFIGURATIONS Debug DESTINATION Debug/lib)