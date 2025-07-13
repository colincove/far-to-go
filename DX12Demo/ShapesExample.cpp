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
	ShapesExample::ShapesExample(std::shared_ptr<DX12> dx12) : AbstractExample(dx12),
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
		UpdateMainPassCB(gameTime);

		// Cycle through the circular frame resource array.
		mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) %
			s_numFrameResources;
		mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();
		// Has the GPU finished processing the commands of the current frame
		// resource. If not, wait until the GPU has completed commands up to
		// this fence point.
		const UINT64 completedFenceValue = m_dx12->mFence->GetCompletedValue();
		if (mCurrFrameResource->mFence != 0 && completedFenceValue < mCurrFrameResource->mFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			m_dx12->mFence->SetEventOnCompletion(mCurrFrameResource->mFence, eventHandle);
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}

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
		UINT objCBByteSize = Math::CalcConstantBufferByteSize(sizeof(ObjectConstants));
		auto objectCB = mCurrFrameResource->mObjectConstants->Resource();

		// For each render item...
		for (size_t i = 0; i < ritems.size(); ++i)
		{
			auto ri = ritems[i];
			const auto& vbv = ri->Geo->VertexBufferView();
			const auto& ibv = ri->Geo->IndexBufferView();
			cmdList->IASetVertexBuffers(0, 1, &vbv);
			cmdList->IASetIndexBuffer(&ibv);
			cmdList->IASetPrimitiveTopology(ri->PrimitiveType);
			// Offset to the CBV in the descriptor heap for this object and
			// for this frame resource.
			UINT cbvIndex = mCurrFrameResourceIndex * (UINT)mOpaqueRitems.size() + ri->ObjCBIndex;
			auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_dx12->mCbvHeap->GetGPUDescriptorHandleForHeapStart());
			cbvHandle.Offset(cbvIndex, m_dx12->mCbvSrvDescriptorSize);
			cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);
			cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
		}
	}

	ShapesExample::FrameResource::FrameResource(std::shared_ptr<DX12> dx12, UINT passCount, UINT objectCount)
		:mDx12(dx12),
		mPassCount(passCount),
		mObjectCount(objectCount),
		mFence(0),
		mCommandListAllocator(nullptr),
		mPassConstants(nullptr),
		mObjectConstants(nullptr)
	{
		HRESULT result = dx12->mDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(mCommandListAllocator.GetAddressOf()));
		assert(result == S_OK);

		mPassConstants = std::make_unique<UploadBuffer<PassConstants>>(dx12->mDevice.Get(),
			passCount, true);
		mObjectConstants = std::make_unique<UploadBuffer<ObjectConstants>>(dx12->mDevice.Get(),
			objectCount, true);
	}
	
	void ShapesExample::BuildFrameResources()
	{
		for (int i = 0; i < s_numFrameResources; ++i)
		{
			mFrameResources.push_back(std::make_unique<FrameResource>(m_dx12, 1, (UINT)mAllRitems.size()));
		}
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
		RECT winRect;
		assert(GetWindowRect(m_dx12->mhMainWnd, &winRect));
		const int width = winRect.right - winRect.left;
		const int height = winRect.bottom - winRect.top;

		const XMMATRIX view = XMLoadFloat4x4(&mView);
		const XMMATRIX proj = XMLoadFloat4x4(&mProj);
		const XMMATRIX viewProj = XMMatrixMultiply(view, proj);

		XMVECTOR viewDet = XMMatrixDeterminant(view);
		XMVECTOR projDet = XMMatrixDeterminant(proj);
		XMVECTOR viewProjDet = XMMatrixDeterminant(viewProj);

		const XMMATRIX invView = XMMatrixInverse(&viewDet, view);
		const XMMATRIX invProj = XMMatrixInverse(&projDet, proj);
		const XMMATRIX invViewProj = XMMatrixInverse(&viewProjDet, viewProj);

		XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
		XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
		XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
		XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
		XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
		XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
		//mMainPassCB.EyePosW = mEyePos; cannot find this deifned anywhere in class or book pages
		mMainPassCB.RenderTargetSize = XMFLOAT2((float) width, (float)height);
		mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / width, 1.0f
			/ height);
		mMainPassCB.NearZ = 1.0f;
		mMainPassCB.FarZ = 1000.0f;
		mMainPassCB.TotalTime = gameTime.TotalSeconds();
		mMainPassCB.DeltaTime = gameTime.DeltaSeconds();
		//auto currPassCB = mCurrFrameResource->PassCB.get();
		//currPassCB->CopyData(0, mMainPassCB);
	}

	void ShapesExample::BuildGeometryData()
	{
		mBoxGeomIdx = mGeometryData.AddEntry(
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

		mMeshGeometry->VertexByteStride = mGeometryData.GetEntry(mBoxGeomIdx).geometry->GetVertexSize();
		mMeshGeometry->VertexBufferByteSize = mGeometryData.GetVertexBufferSize();
		mMeshGeometry->IndexFormat = DXGI_FORMAT_R16_UINT;
	}

	void ShapesExample::BuildRenderItems()
	{
		const GeometryDataEntry& boxEntry = mGeometryData.GetEntry(mBoxGeomIdx);
		const GeometryDataEntry& sphereEntry = mGeometryData.GetEntry(mSphereGeomIdx);
		const GeometryDataEntry& coneEntry = mGeometryData.GetEntry(mConeGeomIdx);

		auto boxRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&boxRitem->World,
			XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, 0.5f,
				0.0f));
		boxRitem->ObjCBIndex = 0;
		boxRitem->Geo = mMeshGeometry.get();
		boxRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		boxRitem->IndexCount = boxEntry.submesh.IndexCount;
		boxRitem->StartIndexLocation = boxEntry.submesh.StartIndexLocation;
		boxRitem->BaseVertexLocation = boxEntry.submesh.BaseVertexLocation;
		mAllRitems.push_back(std::move(boxRitem));

		auto sphereRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&sphereRitem->World,
			XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, 0.5f,
				0.0f));
		sphereRitem->ObjCBIndex = 0;
		sphereRitem->Geo = mMeshGeometry.get();
		sphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		sphereRitem->IndexCount = sphereEntry.submesh.IndexCount;
		sphereRitem->StartIndexLocation = sphereEntry.submesh.StartIndexLocation;
		sphereRitem->BaseVertexLocation = sphereEntry.submesh.BaseVertexLocation;
		mAllRitems.push_back(std::move(sphereRitem));

		// Build the columns and spheres in rows as in Figure 7.6.
		UINT objCBIndex = 2;
		for (int i = 0; i < 5; ++i)
		{
			auto leftCylRitem = std::make_unique<RenderItem>();
			auto rightCylRitem = std::make_unique<RenderItem>();
			auto leftSphereRitem = std::make_unique<RenderItem>();
			auto rightSphereRitem = std::make_unique<RenderItem>();

			XMMATRIX leftCylWorld = XMMatrixTranslation(-5.0f, 1.5f, -10.0f +
				i * 5.0f);
			XMMATRIX rightCylWorld = XMMatrixTranslation(+5.0f, 1.5f, -10.0f +
				i * 5.0f);
			XMMATRIX leftSphereWorld = XMMatrixTranslation(-5.0f, 3.5f, -10.0f +
				i * 5.0f);
			XMMATRIX rightSphereWorld = XMMatrixTranslation(+5.0f, 3.5f, -10.0f
				+ i * 5.0f);

			XMStoreFloat4x4(&leftCylRitem->World, rightCylWorld);
			leftCylRitem->ObjCBIndex = objCBIndex++;
			leftCylRitem->Geo = mMeshGeometry.get();
			leftCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			leftCylRitem->IndexCount = coneEntry.submesh.IndexCount;
			leftCylRitem->StartIndexLocation = coneEntry.submesh.StartIndexLocation;
			leftCylRitem->BaseVertexLocation = coneEntry.submesh.BaseVertexLocation;
			XMStoreFloat4x4(&rightCylRitem->World, leftCylWorld);
			rightCylRitem->ObjCBIndex = objCBIndex++;
			rightCylRitem->Geo = mMeshGeometry.get();
			rightCylRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			rightCylRitem->IndexCount = coneEntry.submesh.IndexCount;
			rightCylRitem->StartIndexLocation = coneEntry.submesh.StartIndexLocation;
			rightCylRitem->BaseVertexLocation = coneEntry.submesh.BaseVertexLocation;
			XMStoreFloat4x4(&leftSphereRitem->World, leftSphereWorld);
			leftSphereRitem->ObjCBIndex = objCBIndex++;
			leftSphereRitem->Geo = mMeshGeometry.get();
			leftSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			leftSphereRitem->IndexCount = coneEntry.submesh.IndexCount;
			leftSphereRitem->StartIndexLocation = coneEntry.submesh.StartIndexLocation;
			leftSphereRitem->BaseVertexLocation = coneEntry.submesh.BaseVertexLocation;
			XMStoreFloat4x4(&rightSphereRitem->World, rightSphereWorld);
			rightSphereRitem->ObjCBIndex = objCBIndex++;
			rightSphereRitem->Geo = mMeshGeometry.get();
			rightSphereRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			rightSphereRitem->IndexCount = coneEntry.submesh.IndexCount;
			rightSphereRitem->StartIndexLocation = coneEntry.submesh.StartIndexLocation;
			rightSphereRitem->BaseVertexLocation = coneEntry.submesh.BaseVertexLocation;

			mAllRitems.push_back(std::move(leftCylRitem));
			mAllRitems.push_back(std::move(rightCylRitem));
			mAllRitems.push_back(std::move(leftSphereRitem));
			mAllRitems.push_back(std::move(rightSphereRitem));
		}

		// All the render items are opaque in this demo.
		for (auto& e : mAllRitems)
			mOpaqueRitems.push_back(e.get());
	}

	void ShapesExample::BuildDescriptorHeaps()
	{
		UINT objCount = (UINT)mOpaqueRitems.size();
		// Need a CBV descriptor for each object for each frame resource,
		// +1 for the perPass CBV for each frame resource.
		UINT numDescriptors = (objCount + 1) * s_numFrameResources;
		// Save an offset to the start of the pass CBVs. These are the last 3
		mPassCbvOffset = objCount * s_numFrameResources;
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
		cbvHeapDesc.NumDescriptors = numDescriptors;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		m_dx12->mDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_dx12->mCbvHeap));
	}

	void ShapesExample::BuildConstantBufferViews()
	{
		UINT objCBByteSize = Math::CalcConstantBufferByteSize(sizeof(ObjectConstants));
		UINT objCount = (UINT)mOpaqueRitems.size();
		// Need a CBV descriptor for each object for each frame resource.
		for (int frameIndex = 0; frameIndex < s_numFrameResources; ++frameIndex)
		{
			auto objectCB = mFrameResources[frameIndex]->mObjectConstants->Resource();
			for (UINT i = 0; i < objCount; ++i)
			{
				D3D12_GPU_VIRTUAL_ADDRESS cbAddress = objectCB->GetGPUVirtualAddress();
				// Offset to the ith object constant buffer in the current buffer.
				cbAddress += i * objCBByteSize;
				// Offset to the object CBV in the descriptor heap.
				int heapIndex = frameIndex * objCount + i;
				auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dx12->mCbvHeap->GetCPUDescriptorHandleForHeapStart());
				handle.Offset(heapIndex, m_dx12->mCbvSrvDescriptorSize);
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = cbAddress;
				cbvDesc.SizeInBytes = objCBByteSize;
				m_dx12->mDevice->CreateConstantBufferView(&cbvDesc, handle);
			}
		}
		UINT passCBByteSize = Math::CalcConstantBufferByteSize(sizeof(PassConstants));
		// Last three descriptors are the pass CBVs for each frame resource.
		for (int frameIndex = 0; frameIndex < s_numFrameResources;
			++frameIndex)
		{
			auto passCB = mFrameResources[frameIndex]->mPassConstants->Resource();
			// Pass buffer only stores one cbuffer per frame resource.
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = passCB-> GetGPUVirtualAddress();
			// Offset to the pass cbv in the descriptor heap.
			int heapIndex = mPassCbvOffset + frameIndex;
			auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dx12->mCbvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(heapIndex, m_dx12->mCbvSrvDescriptorSize);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;
			cbvDesc.SizeInBytes = passCBByteSize;
			m_dx12->mDevice->CreateConstantBufferView(&cbvDesc, handle);
		}
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
		const blMeshIndexedCatalogue::index idx = mMeshIndexedCatalogue.AddMesh(mesh.GetStorage());
		mEntries.push_back(GeometryDataEntry{ idx, std::make_unique<Mesh>(mesh) });

		GeometryDataEntry& entry = mEntries[mEntries.size()-1];

		blMeshIndexedCatalogue::Entry& meshIndexEntry = mMeshIndexedCatalogue.GetEntry(idx);
		SubmeshGeometry& submesh = entry.submesh;

		submesh.IndexCount = static_cast<UINT>(mesh.GetIndexCount());
		submesh.StartIndexLocation = static_cast<UINT>(meshIndexEntry.IndexOffset);
		submesh.BaseVertexLocation = static_cast<UINT>(meshIndexEntry.VertexOffset);

		mTotalVertices += mesh.GetVertexCount();
		mVertexBufferSize += mesh.GetStorage().GetHeader().GetVertexBufferSize();
		mIndexBufferSize += mesh.GetStorage().GetHeader().GetIndexBufferSize();

		return mEntries.size() - 1;
	}

	ShapesExample::GeometryDataEntry& ShapesExample::GeometryData::GetEntry(ShapesExample::GeometryData::index idx)
	{
		return mEntries[idx];
	}
}