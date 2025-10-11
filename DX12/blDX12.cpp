//BoulderLeaf
#include <blDX12.h>
#include <blSwapChain.h>
#include <blWindow.h>

//DirectX
#include "dxgi1_3.h"
#include <dxgidebug.h>

//Standard
#include <atldef.h>
#include <assert.h>

namespace BoulderLeaf::Graphics::DX12
{
	//NOTE: This format was randomly chosen. Verify correct value later
	const DXGI_FORMAT blDX12::sBackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	const DXGI_FORMAT blDX12::sDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	const int blDX12::SwapChainBufferCount = 2;
	const int blDX12::sSrvHeapSize = 64;

	blDX12::blDX12(std::shared_ptr<blWindow> window) :
		API(window),
		mWindow(window),
		mDevice(nullptr)
	{
#ifdef DEBUG
		ComPtr<ID3D12Debug> debugController;
		DX12_API_CALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();

		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (DX12_API_CALL_SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			//dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

			DX12_API_CALL(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true));
			DX12_API_CALL(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true));
		}
#endif // DEBUG

		mDevice = std::make_shared<blDevice>();
		mCommandQueue =  mDevice->CreateCommandQueue();
		mFactory = std::make_shared<blFactory>(); //TODO: construct factory
		//DX12_API_CALL(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mFactory)));
	}

	void blDX12::OnWindowMessage(MSG msg)
	{

	}

	void blDX12::OnResize()
	{

	}

	std::weak_ptr<blDevice> blDX12::GetDevice()
	{
		return mDevice;
	}

	std::weak_ptr<blCommandQueue> blDX12::GetCommandQueue()
	{
		return mCommandQueue;
	}

	std::weak_ptr<blFactory> blDX12::GetFactory()
	{
		return mFactory;
	}

	std::shared_ptr<blSwapChain> CreateSwapChain(
		const std::shared_ptr<blDevice> device,
		const std::shared_ptr<blCommandQueue> commandQueue,
		const std::shared_ptr<blFactory> factory,
		const std::shared_ptr<blWindow> window)
	{
		return std::make_shared<blSwapChain>(
			device->GetDX12Device(),
			commandQueue->GetDX12CommandQueue(),
			factory->GetDX12Factory(),
			blDX12::sBackbufferFormat,
			window->GetWindowHandle());
	}
}