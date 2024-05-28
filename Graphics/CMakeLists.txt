add_library(
	Graphics 
	graphics.cpp
	blGraphicsAPI.h
	blHeadless.h
	blHeadless.cpp
	blVirtualCamera.h
	blVirtualCamera.cpp)

target_include_directories(Graphics
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(
	Graphics
	PUBLIC DX12
)

install(TARGETS Graphics CONFIGURATIONS Debug DESTINATION Debug/lib)