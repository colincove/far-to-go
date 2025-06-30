add_library(
	Core
	blCore.h
	blCore.cpp
	blTypes.h)

set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Core
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

install(TARGETS Core CONFIGURATIONS Debug DESTINATION Debug/lib)