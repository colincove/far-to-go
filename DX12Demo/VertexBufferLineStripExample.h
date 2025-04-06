#pragma once

#include <array>
#include <blDX12.h>
#include <blDx12VertexData.h>
#include "AbstractExample.h"
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class VertexBufferLineStripExample : public AbstractExample
	{
	private:
		 std::array<Vertex, 8> mVertices;
		 ComPtr<ID3D12Resource> mVertexBufferGPU;
		 ComPtr<ID3D12Resource> mVertexBufferUpload;
		 D3D12_VERTEX_BUFFER_VIEW mBufferView;
		 D3D12_VERTEX_BUFFER_VIEW mBufferViews[1];
	public:
		VertexBufferLineStripExample(std::shared_ptr<DX12> dx12);
	public:
		void Update() override;
		void Draw() override;
	};
}