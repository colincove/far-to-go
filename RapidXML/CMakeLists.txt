add_library(
	RapidXML
	rapidxml-1.13/rapidxml.hpp
	rapidxml-1.13/rapidxml_iterators.hpp
	rapidxml-1.13/rapidxml_print.hpp
	rapidxml-1.13/rapidxml_utils.hpp
	main_test.cpp)

# Note: I need this because I did not include any *.cpp files to be linked. 
# I had renamed them all to *.hpp. 
set_target_properties(RapidXML PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(RapidXML
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rapidxml-1.13
        )

install(TARGETS RapidXML CONFIGURATIONS Debug DESTINATION Debug/lib)