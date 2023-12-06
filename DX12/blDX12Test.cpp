//DirectX
#include <directx/d3d12.h>
#include <directx/d3d12video.h>
#include <directx/dxcore.h>
#include <directx/d3dx12.h>
#include "dxguids/dxguids.h"

//BoulderLeaf
#include "blDX12Test.h"

//Standard
#include <iostream>
#include <atlstr.h>

namespace BoulderLeaf::Graphics::DX12::Test
{
	namespace
	{
		const CString ctab = "    ";

		CString CreateTabs(const unsigned int indentation)
		{
			CString result;

			for (int i = 0; i < indentation; ++i)
			{
				result += ctab;
			}

			return result;
		}
		
		const CString ctabs[5]
		{
			CreateTabs(0),
			CreateTabs(1),
			CreateTabs(2),
			CreateTabs(3),
			CreateTabs(4)
		};

		void LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter);
		void LogOutputDisplayModes(ComPtr<IDXGIOutput> output, DXGI_FORMAT format);
	}

	void LogDevices()
	{
		ComPtr<IDXGIFactory> pFactory = nullptr;
		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));

		UINT i = 0;
		ComPtr<IDXGIAdapter> adapter = nullptr;
		std::vector<ComPtr<IDXGIAdapter>> adapterList;

		while (pFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);

			std::cout << "Adapter: " << CString(desc.Description) << "\n";
			LogAdapterOutputs(adapter);
			adapterList.push_back(adapter);
			++i;
		}
	}

	void CreateCommandList()
	{
		ComPtr<ID3D12CommandQueue> commandQueue;
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		ComPtr<ID3D12Device> device;

		//TODO
		//HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
		//device->CreateCommandQueue(&desc, __uuidof(ID3D12CommandQueue), (void**)(&commandQueue));
	}

	namespace
	{
		void LogAdapterOutputs(ComPtr<IDXGIAdapter> adapter)
		{
			UINT i = 0;
			ComPtr<IDXGIOutput> output;

			while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_OUTPUT_DESC desc;
				output->GetDesc(&desc);
				std::cout << ctabs[1] << "Ouput: " << CString(desc.DeviceName) << "\n";
				LogOutputDisplayModes(output, DXGI_FORMAT_B8G8R8A8_UNORM);
				++i;
			}
		}

		void LogOutputDisplayModes(ComPtr<IDXGIOutput> output, DXGI_FORMAT format)
		{
			UINT count = 0;
			UINT flags = 0;

			//call with nullptr list to return count
			output->GetDisplayModeList(format, flags, &count, nullptr);

			std::vector<DXGI_MODE_DESC> modeList(count);
			output->GetDisplayModeList(format, flags, &count, &modeList[0]);

			for (const DXGI_MODE_DESC& mode : modeList)
			{
				UINT n = mode.RefreshRate.Numerator;
				UINT d = mode.RefreshRate.Denominator;

				std::cout << ctabs[2] << "Width: " << mode.Width << "\n";
				std::cout << ctabs[2] << "Heght: " << mode.Height << "\n";
				std::cout << ctabs[2] << "RefreshRate: " << n << "/" << d << "\n";
			}
		}
	}
}