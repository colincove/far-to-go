#pragma once

#include <blDevice.h>
#include <blFactory.h>
#include <blCommandQueue.h>
#include <blCommandListAllocator.h>
#include <blSwapChain.h>
#include <blCommandList.h>
#include <blRenderGroup.h>
#include <blDepthBuffer.h>
#include <blFence.h>
#include <blConstantBufferDescriptorHeap.h>
#include <blGlobalRenderFrameContext.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct blRenderGroupData
	{
		std::unique_ptr<blCommandListAllocator> commandListAllocator;
		std::unique_ptr<blCommandList> commandList;
	};

	//TODO do this for ALL data here. c++ files must include the type if they want to use it. 
	//but it is bad practice ot have this many header files included when they are not needed. Slow build times. 
	class blDX12;
	class blDX12ConstantBufferWithPassCache;
	class blDX12ResourceCacheGlobalInterface;

	struct blGlobalRenderData
	{
		friend blDX12;

		blDX12ResourceCacheGlobalInterface* resourceCacheGlobalInterface;
		std::unique_ptr<blDevice> device;
		std::unique_ptr<blCommandQueue> commandQueue;
		std::unique_ptr<blFactory> factory;
		std::unique_ptr<blSwapChain> swapChain;
		std::unique_ptr<blDepthBuffer> depthBuffer;
		std::unique_ptr<blGlobalRenderFrameContext> globalRenderFrameContext;

		blRenderGroupData renderGroupData[blRenderGroups::MaxValue];

		D3D12_VIEWPORT viewPort;
		D3D12_RECT scissorRect;
	};
}