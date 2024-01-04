add_subdirectory(DirectX-Headers)

add_library(
	DX12 
	blDX12Core.h
	blDX12.h
	blDX12.cpp
	blDX12Test.h
	blDX12Test.cpp)

target_include_directories(DX12
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(DX12 PRIVATE
	DirectX-Headers 
	DirectX-Guids 
	${dxlibs}
	d3d12.lib
	dxguid.lib)

install(TARGETS DX12 CONFIGURATIONS Debug DESTINATION Debug/lib)