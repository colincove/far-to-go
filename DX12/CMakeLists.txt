add_subdirectory(DirectX-Headers)

add_library(
	DX12 
	blDX12Core.h
	dx12.cpp
	blDX12Test.h
	blDX12Test.cpp)

target_include_directories(DX12
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(DX12 PRIVATE
	DirectX-Headers DirectX-Guids ${dxlibs})

target_compile_definitions(
	Math
	PUBLIC
	$<$<CONFIG:Debug>:BL_DX12_DEBUG> #enables debug tools
)

install(TARGETS DX12 CONFIGURATIONS Debug DESTINATION Debug/lib)