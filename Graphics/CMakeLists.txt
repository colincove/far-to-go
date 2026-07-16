add_library(
	Graphics 
	blColour.h
	graphics.cpp
	blBuffer.cpp
	blBuffer.h
	blBufferTypes.cpp
	blBufferTypes.h
	blCompositeMesh.cpp
	blCompositeMesh.h
	blGraphicsCore.h
	blGraphicsAPI.h
	blGraphicsAPI.cpp
	blGraphicsAPIImpl.h
	blGraphicsAPIImpl.cpp
	blGraphicsAPI_LegacyV1.h
	blGraphicsAPI_LegacyV1.cpp
	blHeadless.h
	blHeadless.cpp
	blMaterial.h
	blMaterial.cpp
	blShader.h
	blShader.cpp
	blMesh.h
	blMesh.cpp
	blMeshIndexedCatalogue.h
	blMeshIndexedCatalogue.cpp
	blMeshLibrary.h
	blMeshLibrary.cpp
	blImageFormat.h
	blRenderScene.h
	blRenderScene.cpp
	blVirtualCamera.h
	blVirtualCamera.cpp
	blRenderGroup.h
	blRenderGroup.cpp
	Natvis/blMeshStorageHeader.natvis
	Config/resourceManager.xml
	Config.cmake)

target_include_directories(Graphics
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/../Core
		${CMAKE_CURRENT_SOURCE_DIR}/../Math
		PRIVATE
		./
		../Math
		../Core
		../RapidXML/rapidxml-1.13
		PUBLIC
		./Resources
		./RenderItems
        )

target_link_libraries(
	Graphics
	PUBLIC Math
	DX12
	Core
	CoreApplication
	RapidXML
)

source_group("RenderItems/Headers Files"
	FILES 
	RenderItems/blRenderItemMesh.h
	RenderItems/blRenderItem.h)

source_group("RenderItems/Source Files"
	FILES 
	RenderItems/blRenderItem.cpp)

source_group("Natvis"
	FILES 
	Natvis/blMeshStorageHeader.natvis)

source_group("Config"
	FILES 
	Config/resourceManager.xml)

#source_group("RenderItems/Sources Files" ./RenderItems)

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