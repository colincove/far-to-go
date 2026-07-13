#pragma once

#include "d3dcompiler.h"
#include <directxcollision.h>
//C:\Users\colin\Documents\far - to - go\DirectX - Headers\include\directx\d3dx12_root_signature.h
#include "../DirectX-Headers/include/directx/d3dx12.h"

#include <dxgi.h>
#include <d3d12.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>
#include <blDX12Core.h>
#include <blDevice.h>
#include <memory>
#include <blShader.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blRootSignature
	{
	public:
		blRootSignature(blDevice* device, const blShader& shader);
		ComPtr<ID3D12RootSignature> GetRootSignature() const { return mRootSignature; }
	private:
		ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	};
}