#include <blDX12BufferData.h>
#include <blDX12ReadFile.h>
#include <format>
#include <DirectXMath.h>
#include <blDX12Buffer.h>

namespace BoulderLeaf::Graphics::DX12
{
	blDX12BufferDataCache::blDX12BufferDataCache(std::shared_ptr<blDevice> device)
		: mDevice(device), blDX12ResourceDataCache()
	{

	}

	void blDX12BufferDataCache::InitializeCache(
		const blDataBufferInterfaceResource& resource,
		blDX12BufferData& cache)
	{
		using namespace DirectX;

		cache.dataBuffer = blBasicDataBuffer(
			resource.GetData(),
			DX12::DX12BufferAdapter::Get());

		cache.uploadBuffer = std::make_shared<blUploadBuffer>(
			mDevice.get()->GetDX12Device().Get(),
			static_cast<UINT>(cache.dataBuffer.GetElementSize()),
			static_cast<UINT>(resource.GetData().Count()),
			true);

		cache.uploadBuffer->CopyData(0, cache.dataBuffer.Get(0));
	}
}