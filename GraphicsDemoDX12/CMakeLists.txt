add_executable(
	GraphicsDemoDX12
	main.cpp
	DemoScene01.cpp
	DemoScene01.h
	DemoScene02.cpp
	DemoScene02.h
	Config/resourceManager.xml
	)

source_group("Config"
	FILES 
	Config/resourceManager.xml)

target_link_libraries(
	GraphicsDemoDX12
	PRIVATE
	DX12
	Metrics
	Graphics
	Imgui
	CoreApplication
)

target_include_directories(GraphicsDemoDX12
	PRIVATE
	./
	../Graphics
)

#TODO: install directory based on build type
install(TARGETS GraphicsDemoDX12 CONFIGURATIONS Debug DESTINATION Debug)

bl_copy_configs_to_brinary_dir(
	${CMAKE_CURRENT_SOURCE_DIR}/../Graphics/Config
	${CMAKE_CURRENT_SOURCE_DIR}/Config)