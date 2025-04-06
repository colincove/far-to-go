add_library(
	Metrics
	blTime.h
	blTime.cpp
	blPIX.h
	blPIX.cpp
	)

# Note: I need this because I did not include any *.cpp files to be linked. 
# I had renamed them all to *.inl. 
set_target_properties(Metrics PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Metrics
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_compile_definitions(
	Metrics
	PUBLIC
	$<$<CONFIG:Release>:NDEBUG> #disable asserts in Release configuration
)

install(TARGETS Metrics CONFIGURATIONS Debug DESTINATION Debug/lib)