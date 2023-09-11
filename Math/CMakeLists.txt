add_library(
	blMath
	blMath.inl
	blLine.inl
	blLineSegment.inl
	blMatrix4x4.inl
	blPlane.inl
	blPoint3.inl
	blQuad.inl
	blQuaternion.inl
	blRay.inl
	blRect.inl
	blVector2.inl
	blVector3.inl
	)

# Note: I need this because I did not include any *.cpp files to be linked. 
# I had renamed them all to *.inl. 
set_target_properties(blMath PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(blMath
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )


install(TARGETS blMath CONFIGURATIONS Debug DESTINATION Debug/lib)