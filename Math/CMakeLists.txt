add_library(
	Math 
	blMath.inl
	blMathFloat.h
	blLine.inl
	blLineSegment.inl
	blMatrix4x4.inl
	blPlane.inl
	blQuad.inl
	blRay.inl
	blRect.inl
	blVector2.inl
	blVector3.inl
	blVector3.cpp
	)

# Note: I need this because I did not include any *.cpp files to be linked. 
# I had renamed them all to *.inl. 
set_target_properties(Math PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Math
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )


install(TARGETS Math CONFIGURATIONS Debug DESTINATION Debug/lib)