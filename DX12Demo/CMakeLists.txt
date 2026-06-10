add_executable(
	DX12Demo
	main.cpp
	AbstractExample.h
	AbstractExample.cpp
	BoxExample.h
	BoxExample.cpp
	Examples.h
	Examples.cpp
	VertexBufferLineStripExample.h
	VertexBufferLineStripExample.cpp
	IndexBufferExample.h
	IndexBufferExample.cpp
	ImguiExample.cpp
	ImguiExample.h
	ShapesExample.h
	ShapesExample.cpp
	)

target_link_libraries(
	DX12Demo 
	PUBLIC
	DX12
	Metrics
	Graphics
	Imgui
)

install(TARGETS DX12Demo CONFIGURATIONS Debug DESTINATION Debug)

bl_copy_configs_to_brinary_dir(
	${CMAKE_CURRENT_SOURCE_DIR}/../Graphics/Config)