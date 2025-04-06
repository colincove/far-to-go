add_library(
	Imgui 
	source/imconfig.h
	source/imgui.cpp
	source/imgui.h
	source/imgui_demo.cpp
	source/imgui_draw.cpp
	source/imgui_internal.h
	source/imgui_tables.cpp
	source/imgui_widgets.cpp
	source/imstb_rectpack.h
	source/imstb_textedit.h
	source/imstb_truetype.h
	source/backends/imgui_impl_dx12.cpp
	source/backends/imgui_impl_dx12.h
	source/backends/imgui_impl_win32.cpp
	source/backends/imgui_impl_win32.h
	)

target_include_directories(Imgui
        PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/source
		${CMAKE_CURRENT_SOURCE_DIR}/source/backends
        )

install(TARGETS Imgui CONFIGURATIONS Debug DESTINATION Debug/lib)