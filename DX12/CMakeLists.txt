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
	blGlobalRenderFrameContext.h
	blGlobalRenderFrameContext.cpp
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
	blDX12Imgui.cpp
	blDX12Imgui.h
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
	blDX12Shader.cpp
	blDX12Shader.h
	blPSO.cpp
	blPSO.h
	blDepthBuffer.cpp
	blDepthBuffer.h
	blConstantBufferDescriptorHeap.cpp
	blConstantBufferDescriptorHeap.h
	blUploadBuffer.cpp
	blUploadBuffer.h

	ResourceCache/blDX12ResourceCache.cpp
	ResourceCache/blDX12ResourceCache.h
	ResourceCache/blDX12ResourceCachesImpl.cpp
	ResourceCache/blDX12ResourceCachesImpl.h
	ResourceCache/blDX12ResourceCacheInterface.cpp
	ResourceCache/blDX12ResourceCacheInterface.h
	ResourceCache/blDX12ResourceCacheTemplate.h

	RenderComponents/blRenderComponent.cpp
	RenderComponents/blRenderComponent.h
	RenderComponents/blMeshRenderComponent.cpp
	RenderComponents/blMeshRenderComponent.h
	RenderComponents/blMeshInstancedRenderComponent.cpp
	RenderComponents/blMeshInstancedRenderComponent.h
	RenderComponents/blCompositeMeshRenderComponent.cpp
	RenderComponents/blCompositeMeshRenderComponent.h
	RenderComponents/blCompositeMeshRenderWithPassConstantsRenderComponent.cpp
	RenderComponents/blCompositeMeshRenderWithPassConstantsRenderComponent.h)

source_group("ResourceCache/Headers Files"
	FILES 
	ResourceCache/blDX12ResourceCache.h
	ResourceCache/blDX12ResourceCachesImpl.h
	ResourceCache/blDX12ResourceCacheInterface.h
	ResourceCache/blDX12ResourceCacheTemplate.h)

source_group("ResourceCache/Source Files"
	FILES 
	ResourceCache/blDX12ResourceCache.cpp
	ResourceCache/blDX12ResourceCachesImpl.cpp
	ResourceCache/blDX12ResourceCacheInterface.cpp)

source_group("RenderComponents/Headers Files"
	FILES 
	RenderComponents/blRenderComponent.h
	RenderComponents/blMeshRenderComponent.h
	RenderComponents/blMeshInstancedRenderComponent.h
	RenderComponents/blCompositeMeshRenderComponent.h
	RenderComponents/blCompositeMeshRenderWithPassConstantsRenderComponent.h)

source_group("RenderComponents/Source Files"
	FILES 
	RenderComponents/blRenderComponent.cpp
	RenderComponents/blMeshRenderComponent.cpp
	RenderComponents/blMeshInstancedRenderComponent.cpp
	RenderComponents/blCompositeMeshRenderComponent.cpp
	RenderComponents/blCompositeMeshRenderWithPassConstantsRenderComponent.cpp)

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
	Imgui
	DirectX-Headers 
	DirectX-Guids 
	${dxlibs}
	d3d12.lib
	dxgi.lib
	dxguid.lib
	d3dcompiler.lib)

install(TARGETS DX12 CONFIGURATIONS Debug DESTINATION Debug/lib)