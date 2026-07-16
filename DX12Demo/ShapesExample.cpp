#include "ShapesExample.h"
#include <memory>
#include "blDX12Core.h"
#include <blDx12VertexBuffer.h>
#include "blDX12UploadBuffer.h"
#include <assert.h>
#include <ctime>
#include <chrono>
#include <blMeshLibrary.h>

namespace
{
	using namespace BoulderLeaf::Graphics::DX12;
}

namespace BoulderLeaf::Graphics::DX12
{
	ShapesExample::ShapesExample(std::shared_ptr<DX12_LegacyV1> dx12) : AbstractExample(dx12),
		mFrameResources(),
		mCurrFrameResource(nullptr),
		mCurrFrameResourceIndex(0),
		mMainPassCB(),
		mAllRitems(),
		mOpaqueRitems(),
		mTransparentRitems(),
		mWorld(Math::Identity4x4()),
		mView(Math::Identity4x4()),
		mProj(Math::Identity4x4()),
		mGeometryData(),
		mMeshGeometry(nullptr)
	{
		BuildFrameResources();
		BuildGeometryData();
		BuildRenderItems();
		BuildDescriptorHeaps();
		BuildConstantBufferViews();
	}

	ShapesExample::~ShapesExample()
	{
		m_dx12->FlushCommandQueue();
	}

	void ShapesExample::Update(const Metrics::blTime& gameTime)
	{

		// [...] Update resources in mCurrFrameResource (like cbuffers).
	}

	void ShapesExample::Draw()
	{
		// [...] Build and submit command lists for this frame.
		// 
		// Advance the fence value to mark commands up to this fence point.
		//mCurrFrameResource->mFence = ++m_dx12->mCurrentFence;

		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to
		// this Signal().
		//m_dx12->mCommandQueue->Signal(m_dx12->mFence.Get(), m_dx12->mCurrentFence);

		// Note that GPU could still be working on commands from previous
		// frames, but that is okay, because we are not touching any frame
		// resources associated with those frames.

		auto cmdListAlloc = mCurrFrameResource->mCommandListAllocator;
		// Reuse the memory associated with command recording.
		// We can only reset when the associated command lists have
		// finished execution on the GPU.
		cmdListAlloc->Reset();
		// A command list can be reset after it has been added to the
		// command queue via ExecuteCommandList.
		// Reusing the command list reuses memory.
		if (mIsWireframe)
		{
			//m_dx12->mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque_wireframe"].Get()));
		}
		else
		{
			//mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get());
		}
		/*m_dx12->mCommandList->RSSetViewports(1, &m_dx12->mViewPort);
		m_dx12->mCommandList->RSSetScissorRects(1, &m_dx12->mScissorRect);
		// Indicate a state transition on the resource usage.
		m_dx12->mCommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(m_dx12->CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
		// Clear the back buffer and depth buffer.
		m_dx12->mCommandList->ClearRenderTargetView(m_dx12->CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
		m_dx12->mCommandList->ClearDepthStencilView(m_dx12->DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		// Specify the buffers we are going to render to.
		m_dx12->mCommandList->OMSetRenderTargets(1, &m_dx12->CurrentBackBufferView(), true, &m_dx12->DepthStencilView());
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_dx12->mCbvHeap.Get() };
		m_dx12->mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps),
			descriptorHeaps);
		m_dx12->mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
		int passCbvIndex = mPassCbvOffset + mCurrFrameResourceIndex;
		auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_dx12->mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		passCbvHandle.Offset(passCbvIndex, mCbvSrvUavDescriptorSize);
		m_dx12->mCommandList->SetGraphicsRootDescriptorTable(1, passCbvHandle);
		DrawRenderItems(m_dx12->mCommandList.Get(), mOpaqueRitems);
		// Indicate a state transition on the resource usage.
		m_dx12->mCommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(m_dx12->CurrentBackBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT));
		// Done recording commands.
		m_dx12->mCommandList->Close();
		// Add the command list to the queue for execution.
		ID3D12CommandList* cmdsLists[] = { m_dx12->mCommandList.Get() };
		m_dx12->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
		// Swap the back and front buffers
		m_dx12->mSwapChain->Present(0, 0);
		m_dx12->mCurrBackBuffer = (m_dx12->mCurrBackBuffer + 1) % SwapChainBufferCount;
		// Advance the fence value to mark commands up to this fence point.
		mCurrFrameResource->Fence = ++m_dx12->mCurrentFence;
		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to this Signal().
		m_dx12->mCommandQueue->Signal(m_dx12->mFence.Get(), m_dx12->mCurrentFence);*/
	}

	void ShapesExample::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
	{
	}

	ShapesExample::FrameResource::FrameResource(std::shared_ptr<DX12_LegacyV1> dx12, UINT passCount, UINT objectCount)
		:mDx12(dx12),
		mPassCount(passCount),
		mObjectCount(objectCount),
		mFence(0),
		mCommandListAllocator(nullptr),
		mPassConstants(nullptr),
		mObjectConstants(nullptr)
	{
	}
	
	void ShapesExample::BuildFrameResources()
	{
	}

	void ShapesExample::UpdateObjectCBs()
	{
		/*auto currObjectCB = mCurrFrameResource->ObjectCB.get();
		for (auto& e : mAllRitems)
		{
			// Only update the cbuffer data if the constants have changed.
			// This needs to be tracked per frame resource.
			if (e->NumFramesDirty > 0)
			{
				XMMATRIX world = XMLoadFloat4x4(&e->World);
				ObjectConstants objConstants;
				XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
				currObjectCB->CopyData(e->ObjCBIndex, objConstants);
				// Next FrameResource need to be updated too.
				e->NumFramesDirty--;
			}
		}*/
	}

	void ShapesExample::UpdateMainPassCB(const Metrics::blTime& gameTime)
	{
	}

	void ShapesExample::BuildGeometryData()
	{
		/*mBoxGeomIdx = mGeometryData.AddEntry(
			Mesh(blMeshStorage::To<StandardVertex, Vertex>(Cube().GetStorage())));

		mConeGeomIdx = mGeometryData.AddEntry(
			Mesh(blMeshStorage::To<StandardVertex, Vertex>(CreateCylinder(
				2, 1, 2, 8, 4).GetStorage())));

		mSphereGeomIdx = mGeometryData.AddEntry(
			Mesh(blMeshStorage::To<StandardVertex, Vertex>(CreateGeosphere(1, 2).GetStorage())));

		const std::vector<const blMeshStorage*> storage =
		{
			&mGeometryData.GetEntry(mBoxGeomIdx).geometry->GetStorage(),
			&mGeometryData.GetEntry(mConeGeomIdx).geometry->GetStorage(),
			&mGeometryData.GetEntry(mSphereGeomIdx).geometry->GetStorage()
		};

		std::unique_ptr<byte[]> vertexBuffer = blMeshStorage::CreateVertexBuffer(storage);
		std::unique_ptr<blMeshStorage::index[]> indexBuffer = blMeshStorage::CreateIndexBuffer(storage);

		mMeshGeometry = std::make_unique<MeshGeometry<blMeshStorage>>();

		D3DCreateBlob(mGeometryData.GetVertexBufferSize(), &mMeshGeometry->VertexBufferCPU);
		CopyMemory(mMeshGeometry->VertexBufferCPU->GetBufferPointer(), vertexBuffer.get(), mGeometryData.GetVertexBufferSize());
		D3DCreateBlob(mGeometryData.GetIndexBufferSize(), &mMeshGeometry->IndexBufferCPU);
		CopyMemory(mMeshGeometry->IndexBufferCPU->GetBufferPointer(), indexBuffer.get(), mGeometryData.GetIndexBufferSize());
		
		mMeshGeometry->VertexBufferGPU = CreateDefaultBuffer(
			m_dx12.get()->mDevice.Get(),
			m_dx12.get()->mCommandList.Get(),
			vertexBuffer.get(),
			mGeometryData.GetVertexBufferSize(),
			mMeshGeometry->VertexBufferUploader);

		mMeshGeometry->VertexBufferGPU = CreateDefaultBuffer(
			m_dx12.get()->mDevice.Get(),
			m_dx12.get()->mCommandList.Get(),
			indexBuffer.get(),
			mGeometryData.GetIndexBufferSize(),
			mMeshGeometry->IndexBufferUploader);

		mMeshGeometry->VertexByteStride = static_cast<UINT>(mGeometryData.GetEntry(mBoxGeomIdx).geometry->GetVertexSize());
		mMeshGeometry->VertexBufferByteSize = static_cast<UINT>(mGeometryData.GetVertexBufferSize());
		mMeshGeometry->IndexFormat = DXGI_FORMAT_R16_UINT;*/
	}

	void ShapesExample::BuildRenderItems()
	{
	}

	void ShapesExample::BuildDescriptorHeaps()
	{
	}

	void ShapesExample::BuildConstantBufferViews()
	{
	}

	ShapesExample::GeometryData::GeometryData()
		: mMeshIndexedCatalogue(),
		mEntries(),
		mTotalVertices(0),
		mVertexBufferSize(0),
		mIndexBufferSize(0)
	{
	}
	
	ShapesExample::GeometryData::index ShapesExample::GeometryData::AddEntry(const Mesh& mesh)
	{
		return 0;
	}

	ShapesExample::GeometryDataEntry& ShapesExample::GeometryData::GetEntry(ShapesExample::GeometryData::index idx)
	{
		return mEntries[idx];
	}
}