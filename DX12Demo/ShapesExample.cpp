#include "ShapesExample.h"
#include <memory>
#include "blDX12Core.h"
#include "blDX12UploadBuffer.h"
#include <assert.h>
#include <ctime>
#include <chrono>

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
		mProj(Math::Identity4x4())
	{
		BuildFrameResources();
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
		mCurrFrameResource->mFence = ++m_dx12->mCurrentFence;

		// Add an instruction to the command queue to set a new fence point.
		// Because we are on the GPU timeline, the new fence point won’t be
		// set until the GPU finishes processing all the commands prior to
		// this Signal().
		m_dx12->mCommandQueue->Signal(m_dx12->mFence.Get(), m_dx12->mCurrentFence);

		// Note that GPU could still be working on commands from previous
		// frames, but that is okay, because we are not touching any frame
		// resources associated with those frames.
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
}