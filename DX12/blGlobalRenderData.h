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
		std::unique_ptr<blCommandListAllocator> commandListAllocator;
		std::unique_ptr<blCommandList> commandList;
		std::unique_ptr<blDX12MeshDataDeviceCache> meshDataDeviceCache;
		std::unique_ptr<blCompositeMeshDataCache> compositeMeshStorageCache;
	};

	//TODO do this for ALL data here. c++ files must include the type if they want to use it. 
	//but it is bad practice ot have this many header files included when they are not needed. Slow build times. 
	class blDX12;
	class blDX12ConstantBufferWithPassCache;
	class blDX12ElementUploadBufferCache;
	class blDX12ResourceCacheGlobalInterface;

	struct blGlobalRenderData
	{
		friend blDX12;

		blDX12ResourceCacheGlobalInterface* resourceCacheGlobalInterface;
		std::unique_ptr<blDevice> device;
		std::unique_ptr<blCommandQueue> commandQueue;
		std::unique_ptr<blFactory> factory;
		std::unique_ptr<blSwapChain> swapChain;
		std::unique_ptr<blShaderCache> shaderCache;
		std::unique_ptr<blDX12MeshStorageCache> meshStorageCache;
		std::unique_ptr<blDX12ConstantBufferCache> constantBufferCache;
		std::unique_ptr<blDX12ConstantBufferWithPassCache> constantBufferWithPassCache;
		std::unique_ptr<blDX12BufferDataCache> bufferCache;
		std::unique_ptr<blDX12VertexAndPassUploadBufferCache> vertexAndPassUploadBufferCache;
		std::unique_ptr<blDX12BufferElementCache> bufferElementCache;
		std::unique_ptr<blPSOCache> mPSOCache;
		std::unique_ptr<blDepthBuffer> depthBuffer;
		std::unique_ptr<blGlobalRenderFrameContext> globalRenderFrameContext;
		std::unique_ptr<blDX12ElementUploadBufferCache> elementUploadBufferCache;

		blRenderGroupData renderGroupData[blRenderGroups::MaxValue];

		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
	};
}