#pragma once

#include <blGraphicsAPI.h>
#include <memory>
#include <blDevice.h>
#include <blFactory.h>
#include <blCommandQueue.h>
#include <blCommandListAllocator.h>
#include <dxgiformat.h>
#include <blWindow.h>
#include <blSwapChain.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12 : public API
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
		std::shared_ptr<blCommandQueue> mCommandQueue;
		std::shared_ptr<blFactory> mFactory;
		std::shared_ptr<Core::blWindow> mWindow;
		std::shared_ptr<blSwapChain> mSwapChain;
	public:
		std::shared_ptr<blDevice> GetDevice();
		std::shared_ptr<blCommandQueue> GetCommandQueue();
		std::shared_ptr<blFactory> GetFactory();
		std::shared_ptr<blCommandListAllocator> GetCommandListAllocator();
		std::shared_ptr<blSwapChain> GetSwapChain();
	public:
		//NOTE: This format was randomly chosen. Verify correct value later
		const static DXGI_FORMAT sBackbufferFormat;
		const static DXGI_FORMAT sDepthStencilFormat;

		const static int SwapChainBufferCount;
		const static int sSrvHeapSize;

	public:
		blDX12(std::shared_ptr<Core::blWindow> window);
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	};
}