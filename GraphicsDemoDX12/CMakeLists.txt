add_executable(
	GraphicsDemoDX12
	main.cpp
	DemoScene01.cpp
	DemoScene01.h
	)

target_link_libraries(
	GraphicsDemoDX12
	PUBLIC
	DX12
	Metrics
	Graphics
	Imgui
)

target_include_directories(GraphicsDemoDX12
		PRIVATE
		./
		../Graphics
)

install(TARGETS GraphicsDemoDX12 CONFIGURATIONS Debug DESTINATION Debug)