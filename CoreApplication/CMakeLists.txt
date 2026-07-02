add_library(
	CoreApplication
	blGameLoop.h
	blGameLoop.cpp
	blWindow.h
	blWindow.cpp
	blConfig.h
	blConfig.cpp
	Resources/blResources.cpp
	Resources/blResourceHandle.cpp
	Resources/blResourcesExprimental.h
	Resources/blResourceGuid.h
	Resources/blResourceHandle.h
	Resources/blResourceHandleOfType.h
	CoreApplicationFunctions.cmake)

set_target_properties(CoreApplication PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(CoreApplication
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/../Math
		PRIVATE
		./
		../Math
		PUBLIC
		../Core
        )

target_link_libraries(
	CoreApplication
	PUBLIC Metrics
	Math
)

source_group("Resources/Headers Files"
	FILES 
	Resources/blResourcesExprimental.h
	Resources/blResourceGuid.h
	Resources/blResourceHandle.h
	Resources/blResourceHandleOfType.h)

source_group("Resources/Source Files"
	FILES 
	Resources/blResources.cpp
	Resources/blResourceHandle.cpp)

install(TARGETS CoreApplication CONFIGURATIONS Debug DESTINATION Debug/lib)