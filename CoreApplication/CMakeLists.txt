add_library(
	CoreApplication
	blGameLoop.h
	blGameLoop.cpp
	blWindow.h
	blWindow.cpp)

set_target_properties(CoreApplication PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(CoreApplication
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(
	CoreApplication
	PUBLIC Metrics
)

install(TARGETS CoreApplication CONFIGURATIONS Debug DESTINATION Debug/lib)