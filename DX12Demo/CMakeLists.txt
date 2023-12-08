add_executable(
	DX12Demo
	main.cpp
	)

target_link_libraries(
	DX12Demo 
	PUBLIC DX12
)

install(TARGETS DX12Demo CONFIGURATIONS Debug DESTINATION Debug)