add_library(
	Math
	blMath.inl
	blMathUtils.cpp #We need at least 1 *.cpp file. Otherwise, the lib will not get generated, and cannot be linked
	#Alternatively, we remove linking to the Math lib. And make it stricktly an inline code repository. 
	#But I "suspect" that I will want more than just inline code in the future. So lets make it work for now.
	blMath3D.inl
	blLine.inl
	blLineSegment.inl
	blMatrix2x2.inl
	blMatrix2x3.inl
	blMatrix3x3.inl
	blMatrix4x4.inl
	blPlane.inl
	blPoint3.inl
	blQuad.inl
	blQuaternion.inl
	blRay.inl
	blRect.inl
	blVector2.inl
	blVector3.inl
	blVector4.inl
	)

# Note: I need this because I did not include any *.cpp files to be linked. 
# I had renamed them all to *.inl. 
set_target_properties(Math PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Math
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_compile_definitions(
	Math
	PUBLIC
	$<$<CONFIG:Release>:NDEBUG> #disable asserts in Release configuration
)

install(TARGETS Math CONFIGURATIONS Debug DESTINATION Debug/lib)