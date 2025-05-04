#pragma once

#include <array>
#include <blDX12.h>
#include <blDx12VertexData.h>
#include "AbstractExample.h"
#include <memory>

namespace BoulderLeaf::Graphics::DX12
{
	class IndexBufferExample : public AbstractExample
	{
	private:
		std::array<uint64_t, 36> mInstances;
		ComPtr<ID3D12Resource> mIndexBufferGPU;
		ComPtr<ID3D12Resource> mIndexBufferUploader;
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView;

		std::array<Vertex, 7> mVertices;
		ComPtr<ID3D12Resource> mVertexBufferGPU;
		ComPtr<ID3D12Resource> mVertexBufferUpload;
		D3D12_VERTEX_BUFFER_VIEW mBufferView;
		D3D12_VERTEX_BUFFER_VIEW mBufferViews[1];
	public:
		IndexBufferExample(std::shared_ptr<DX12> dx12);
	public:
		void Update(const Metrics::blTime& gameTime) override;
		void Draw() override;
	};
}