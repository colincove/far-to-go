#include <blDX12.h>
#include <blDx12VertexBuffer.h>
#include <memory>
#include "VertexBufferLineStripExample.h"

namespace BoulderLeaf::Graphics::DX12
{
	VertexBufferLineStripExample::VertexBufferLineStripExample(std::shared_ptr<DX12> dx12)
		:mVertices(
			{
				Vertex(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0), XMFLOAT2(0, 0)),
				{ XMFLOAT3(2, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 0, 0, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(2, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 1, 0, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(2, 2, 2), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 1, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(3, 2, 2), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 1, 0, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(3, 3, 2), XMFLOAT3(0, 0, 0), XMFLOAT4(0, 1, 1, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(4, 3, 3), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 1, 1, 0), XMFLOAT2(0, 0) },
				{ XMFLOAT3(4, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT4(1, 1, 1, 0), XMFLOAT2(0, 0) }
			}),
		mVertexBufferGPU(nullptr),
		mVertexBufferUpload(nullptr),
		mBufferView(),
		mBufferViews(),
		AbstractExample(dx12)
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
		mBufferView.SizeInBytes = vbByteSize;
		mBufferView.StrideInBytes = sizeof(Vertex);

		mBufferViews[1] = { mBufferView };
		dx12->mCommandList->IASetVertexBuffers(0, 1, mBufferViews);
		dx12->mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}

	void VertexBufferLineStripExample::Update()
	{
		m_dx12->mCommandList->DrawInstanced(4, 2, 0, 0);
	};

	void VertexBufferLineStripExample::Draw()
	{
		m_dx12->mCommandList->DrawInstanced(4, 2, 0, 0);
	};
}