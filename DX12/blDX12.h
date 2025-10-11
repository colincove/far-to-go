#pragma once

#include <blGraphicsAPI.h>
#include <memory>
#include <blDevice.h>
#include <blFactory.h>
#include <dxgiformat.h>
#include <blWindow.h>
#include <blSwapChain.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12 : public API
	{
	private:
		std::shared_ptr<blDevice> mDevice;
		std::shared_ptr<blCommandQueue> mCommandQueue;
		std::shared_ptr<blFactory> mFactory;
		std::shared_ptr<blWindow> mWindow;
	public:
		std::weak_ptr<blDevice> GetDevice();
		std::weak_ptr<blCommandQueue> GetCommandQueue();
		std::weak_ptr<blFactory> GetFactory();
	public:
		//NOTE: This format was randomly chosen. Verify correct value later
		const static DXGI_FORMAT sBackbufferFormat;
		const static DXGI_FORMAT sDepthStencilFormat;

		const static int SwapChainBufferCount;
		const static int sSrvHeapSize;

	public:
		blDX12(std::shared_ptr<blWindow> window);
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
	};

	std::shared_ptr<blSwapChain> CreateSwapChain(
		const std::shared_ptr<blDevice> device,
		const std::shared_ptr<blCommandQueue> commandQueue,
		const std::shared_ptr<blFactory> factory,
		const std::shared_ptr<blWindow> window);
}