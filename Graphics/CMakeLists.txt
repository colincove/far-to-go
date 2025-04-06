add_library(
	Graphics 
	graphics.cpp
	blGraphicsAPI.h
	blGraphicsAPI.cpp
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

#after adding the include #include <windef.h> I started to get this compile error
#maybe because this is a library. And it does not know what architecture to compile in?

#C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um\winnt.h
#define PROBE_ALIGNMENT( _s ) TYPE_ALIGNMENT( DWORD )
#elif !defined(RC_INVOKED)
#error "No Target Architecture"
#endif

#answer
#include <windows.h> 

#target_compile_definitions(
#	Graphics
#	PUBLIC
#)

install(TARGETS Graphics CONFIGURATIONS Debug DESTINATION Debug/lib)