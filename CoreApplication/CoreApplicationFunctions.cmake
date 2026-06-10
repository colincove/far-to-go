function(bl_copy_configs_to_brinary_dir TARGET) 
	foreach(CONFIG_DIRECTORY_PATH IN LISTS ARGV)
		message(STATUS "Copy configs in this directory: ${CONFIG_DIRECTORY_PATH}")

		#Note: We do not recommend using GLOB to collect a list of source files from your source tree. If no CMakeLists.txt file changes when a source is added or removed then the generated build system cannot know when to ask CMake to regenerate. The CONFIGURE_DEPENDS flag may not work reliably on all generators, or if a new generator is added in the future that cannot support it, projects using it will be stuck. Even if CONFIGURE_DEPENDS works reliably, there is still a cost to perform the check on every rebuild.

		# CONFIGURE_DEPENDS will force a rebuild if any of these files are changes. 
		# without this. changing the files did not results in an updated config directory.
		# Note: We are using GLOB here because we want to copy all the files in the config directory, and we want to be able to add new files without having to update the CMakeLists.txt file. We are not using GLOB to collect source files for our project, which is not recommended.
		file(GLOB CONFIG_FILES CONFIGURE_DEPENDS "${CONFIG_DIRECTORY_PATH}/*")

		# 2. Loop through each discovered file path
		foreach(FILE_PATH IN LISTS CONFIG_FILES)
			message(STATUS "Found Config file: ${FILE_PATH}")
			message(STATUS "     Copy to destination: ${CMAKE_CURRENT_BINARY_DIR}/Config")
			file(COPY ${FILE_PATH}
				DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Config)
		endforeach()
	endforeach()
endfunction()