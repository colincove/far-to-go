#include <blDX12.h>
#include <blDx12VertexBuffer.h>
#include "IndexBufferExample.h"
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	IndexBufferExample::IndexBufferExample(std::shared_ptr<DX12> dx12)
		:AbstractExample(dx12),
		mVertices(
			{
				Vertex(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0, 0)),
				{ XMFLOAT3(2, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 0, 0, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(2, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 1, 0, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(2, 2, 2), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 1, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(3, 2, 2), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 1, 0, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(3, 3, 2), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 1, 1, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(4, 3, 3), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 1, 1, 0), XMFLOAT2(0, 0) }
			}),
		mVertexBufferGPU(nullptr),
		mVertexBufferUpload(nullptr),
		mBufferView(),
		mBufferViews(),
		mInstances({
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7 }),
		mIndexBufferGPU(nullptr),
		mIndexBufferUploader(nullptr),
		mIndexBufferView()
	{
		const UINT64 vbByteSize = mVertices.size() * sizeof(Vertex);

		mVertexBufferGPU = CreateDefaultBuffer(
			dx12->mDevice.Get(),
			dx12->mCommandList.Get(),
			static_cast<void*>(mVertices.data()),
			vbByteSize,
			mVertexBufferUpload
		);

		mBufferView.BufferLocation = mVertexBufferGPU->GetGPUVirtualAddress();
		mBufferView.SizeInBytes = static_cast<UINT>(vbByteSize);
		mBufferView.StrideInBytes = sizeof(Vertex);

		mBufferViews[1] = { mBufferView };
		dx12->mCommandList->IASetVertexBuffers(0, 1, mBufferViews);
		dx12->mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

		const UINT ibByteSize = 36 * sizeof(uint16_t);

		mIndexBufferGPU = CreateDefaultBuffer(
			dx12->mDevice.Get(),
			dx12->mCommandList.Get(),
			static_cast<void*>(mVertices.data()),
			ibByteSize,
			mIndexBufferUploader);

	};

	void IndexBufferExample::Update(const Metrics::blTime& gameTime)
	{
	};

	void IndexBufferExample::Draw()
	{
	};
}