add_executable(
	GraphicsDemoDX12
	main.cpp
	)

target_link_libraries(
	GraphicsDemoDX12
	PUBLIC
	DX12
	Metrics
	Graphics
	Imgui
)

install(TARGETS GraphicsDemoDX12 CONFIGURATIONS Debug DESTINATION Debug)