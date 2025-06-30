add_library(
	DX12 
	blDX12Core.h
	blDX12.h
	blDX12.cpp
	blDX12Math.inl
	blDX12MeshGeometry.h
	blDX12ReadFile.h
	blDX12ReadFile.cpp
	blDX12Test.h
	blDX12Test.cpp
	blDx12VertexBuffer.h
	blDx12VertexBuffer.cpp
	blDx12VertexData.h
	blDx12VertexData.cpp
	blDX12UploadBuffer.h
	)

target_include_directories(DX12
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
		PRIVATE 
		../Core
		../Math
		../Graphics
		./
        )

target_link_libraries(DX12 PRIVATE
	DirectX-Headers 
	DirectX-Guids 
	${dxlibs}
	d3d12.lib
	dxgi.lib
	dxguid.lib
	d3dcompiler.lib)

install(TARGETS DX12 CONFIGURATIONS Debug DESTINATION Debug/lib)