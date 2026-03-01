add_library(
	DX12 
	blCommandList.cpp
	blCommandList.h
	blCommandListAllocator.h
	blCommandListAllocator.cpp
	blCommandQueue.h
	blCommandQueue.cpp
	blConstantBuffer.h
	blConstantBuffer.cpp
	blGlobalRenderData.h
	blDX12Buffer.cpp
	blDX12Buffer.h
	blDX12Core.h
	blDX12Core.cpp
	blDX12.h
	blDX12.cpp
	blDX12_LegacyV1.h
	blDX12_LegacyV1.cpp
	blDevice.cpp
	blDevice.h
	blDescriptorHeap.h
	blDescriptorHeap.cpp
	blDX12Math.inl
	blDX12MeshGeometry.h
	blDX12ReadFile.h
	blDX12ReadFile.cpp
	blDX12Test.h
	blDX12Test.cpp
	blFactory.cpp
	blFactory.h
	blFence.h
	blFence.cpp
	blDx12VertexBuffer.h
	blDx12VertexBuffer.cpp
	blDx12VertexData.h
	blDx12VertexData.cpp
	blDX12UploadBuffer.h
	blRenderContext.h
	blRenderContext.cpp
	blRootSignature.h
	blRootSignature.cpp
	blRenderSceneContext.cpp
	blRenderSceneContext.h
	blPipelineState.h
	blPipelineState.cpp
	blShaderData.h
	blShaderData.cpp
	blSwapChain.h
	blSwapChain.cpp
	blPSO.cpp
	blPSO.h
	blDepthBuffer.cpp
	blDepthBuffer.h
	blConstantBufferDescriptorHeap.cpp
	blConstantBufferDescriptorHeap.h

	RenderComponents/blRenderComponent.cpp
	RenderComponents/blRenderComponent.h
	RenderComponents/blMeshRenderComponent.cpp
	RenderComponents/blMeshRenderComponent.h
	RenderComponents/blMeshInstancedRenderComponent.cpp
	RenderComponents/blMeshInstancedRenderComponent.h

	RenderData/blDX12BufferData.cpp
	RenderData/blDX12BufferData.h
	RenderData/blDX12ShaderCache.cpp
	RenderData/blDX12ShaderCache.h
	RenderData/blDX12MeshDataCache.cpp
	RenderData/blDX12MeshDataCache.h
	RenderData/blDX12ResourceDataCache.cpp
	RenderData/blDX12ResourceDataCache.h
	RenderData/blDX12MeshDataDeviceCache.cpp
	RenderData/blDX12MeshDataDeviceCache.h
	RenderData/blDX12PSODataCache.h
	RenderData/blDX12PSODataCache.cpp
	RenderData/blDX12ConstantBufferCache.h
	RenderData/blDX12ConstantBufferCache.cpp
	)

source_group("Render Components/Headers Files"
	FILES 
	RenderComponents/blRenderComponent.h
	RenderComponents/blMeshRenderComponent.h
	RenderComponents/blMeshInstancedRenderComponent.h)

source_group("Render Components/Source Files"
	FILES 
	RenderComponents/blRenderComponent.cpp
	RenderComponents/blMeshRenderComponent.cpp
	RenderComponents/blMeshInstancedRenderComponent.cpp)

source_group("Render Data/Headers Files"
	FILES 
	RenderData/blDX12BufferData.h
	RenderData/blDX12MeshDataDeviceCache.h
	RenderData/blDX12ShaderCache.h
	RenderData/blDX12MeshDataCache.h
	RenderData/blDX12ResourceDataCache.h
	RenderData/blDX12PSODataCache.h
	RenderData/blDX12ConstantBufferCache.h)

source_group("Render Data/Source Files"
	FILES 
	RenderData/blDX12BufferData.cpp
	RenderData/blDX12MeshDataCache.cpp
	RenderData/blDX12ResourceDataCache.cpp
	RenderData/blDX12MeshDataDeviceCache.cpp
	RenderData/blDX12ShaderCache.cpp
	RenderData/blDX12PSODataCache.cpp
	RenderData/blDX12ConstantBufferCache.cpp)

target_include_directories(DX12
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
		PRIVATE 
		../Core
		../Math
		../Graphics
		./
		PUBLIC
		./RenderComponents/
		./RenderData/
        )

target_link_libraries(DX12 PRIVATE
	CoreApplication
	DirectX-Headers 
	DirectX-Guids 
	${dxlibs}
	d3d12.lib
	dxgi.lib
	dxguid.lib
	d3dcompiler.lib)

install(TARGETS DX12 CONFIGURATIONS Debug DESTINATION Debug/lib)