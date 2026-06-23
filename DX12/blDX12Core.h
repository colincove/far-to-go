#pragma once

//I dont understand why, but without this, CreateDXGIFactory could not link. 
#pragma comment(lib, "dxgi")

#include <wrl.h>
#include <assert.h>
//#include <atldef.h>
#include <dxgiformat.h>
#include <blImageFormat.h>
#include <blGraphicsCore.h>
#include <d3d12.h>

namespace BoulderLeaf::Graphics::DX12
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

#ifdef DEBUG
	#define DX12_API_CALL(a) {HRESULT r = (a); if(FAILED(r)) throw stdruntime_error("DX12 API failed: " + stdto_string((long)r));}
#else
	#define DX12_API_CALL(a) (a)
#endif // DEBUG

#ifdef DEBUG
	#define DX12_API_CALL_SUCCEEDED(a) ((a) == S_OK)
#else
	#define DX12_API_CALL_SUCCEEDED(a) (a)
#endif // DEBUG

	DXGI_FORMAT ConvertImageFormatToDXGIFormat(eImageFormat format);
	D3D12_INPUT_CLASSIFICATION ConvertInputCassificationToD3D12InputClassification(eInputClassification format);
	D3D12_INPUT_ELEMENT_DESC ConvertInputElementDescToD3D12InputElementDesc(const InputElementDesc& desc);

	struct Constants
	{
		//NOTE: This format was randomly chosen. Verify correct value later
		static constexpr  DXGI_FORMAT BackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr  DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		static constexpr  int SwapChainBufferCount = 2;
		static constexpr  int SrvHeapSize = 64; //this will likely not remain a constant. not appropriate.
		static constexpr int FrameResourceCount = 3;
	};
}