#pragma once

#include <blDevice.h>
#include <blFactory.h>
#include <blCommandQueue.h>
#include <blCommandListAllocator.h>
#include <blSwapChain.h>
#include <blDX12ShaderCache.h>
#include <blDX12MeshDataCache.h>
#include <blDX12MeshDataDeviceCache.h>
#include <blCommandList.h>
#include <blDX12PSODataCache.h>
#include <blRenderGroup.h>
#include <blDepthBuffer.h>
#include <blFence.h>
#include <blDX12ConstantBufferCache.h>
#include <blDX12BufferData.h>
#include <blConstantBufferDescriptorHeap.h>
#include <RenderData/blDX12CompositeMeshDataCache.h>
#include <RenderData/blDX12BufferElementDataCache.h>
#include <blGlobalRenderFrameContext.h>
#include <blDX12VertexAndPassUploadBufferCache.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blRenderGroupData
	{
		std::shared_ptr<blCommandListAllocator> commandListAllocator;
		std::shared_ptr<blCommandList> commandList;
		std::shared_ptr<blDX12MeshDataDeviceCache> meshDataDeviceCache;
		std::shared_ptr<blCompositeMeshDataCache> compositeMeshStorageCache;
	};

	//TODO do this for ALL data here. c++ files must include the type if they want to use it. 
	//but it is bad practice ot have this many header files included when they are not needed. Slow build times. 
	class blDX12;
	class blDX12ConstantBufferWithPassCache;
	class blDX12ElementUploadBufferCache;

	struct blGlobalRenderData
	{
		friend blDX12;

		std::shared_ptr<blDevice> device;
		std::shared_ptr<blCommandQueue> commandQueue;
		std::shared_ptr<blFactory> factory;
		std::shared_ptr<blSwapChain> swapChain;
		std::shared_ptr<blShaderCache> shaderCache;
		std::shared_ptr<blDX12MeshStorageCache> meshStorageCache;
		std::shared_ptr<blDX12ConstantBufferCache> constantBufferCache;
		std::shared_ptr<blDX12ConstantBufferWithPassCache> constantBufferWithPassCache;
		std::shared_ptr<blDX12BufferDataCache> bufferCache;
		std::shared_ptr<blDX12VertexAndPassUploadBufferCache> vertexAndPassUploadBufferCache;
		std::shared_ptr<blDX12BufferElementCache> bufferElementCache;
		std::shared_ptr<blPSOCache> mPSOCache;
		std::shared_ptr<blDepthBuffer> depthBuffer;
		std::shared_ptr<blDX12MeshDataDeviceCache> meshDataDeviceCache;
		std::shared_ptr<blCompositeMeshDataCache> compositeMeshStorageCache;
		std::shared_ptr<blGlobalRenderFrameContext> globalRenderFrameContext;
		std::shared_ptr<blDX12ElementUploadBufferCache> elementUploadBufferCache;

		blRenderGroupData renderGroupData[blRenderGroups::MaxValue];

		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
	};
}