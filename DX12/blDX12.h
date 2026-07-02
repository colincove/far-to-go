#pragma once

#include <blGraphicsAPI.h>
#include <memory>
#include <blDevice.h>
#include <blFactory.h>
#include <blCommandQueue.h>
#include <blCommandListAllocator.h>
#include <dxgiformat.h>
#include <blWindow.h>
#include <blSwapChain.h>
#include <map>
#include <set>
#include <blRenderScene.h>
#include <blRenderSceneContext.h>
#include <blGlobalRenderData.h>
#include <blGraphicsAPIImpl.h>
#include <RenderComponents/blMeshRenderComponent.h>
#include <RenderComponents/blMeshInstancedRenderComponent.h>
#include <RenderComponents/blCompositeMeshRenderComponent.h>
#include <RenderComponents/blCompositeMeshRenderWithPassConstantsRenderComponent.h>
#include <blDX12Imgui.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct DX12ResourceState : public ResourceState
	{
	};

	class blDX12 : public blGraphicsAPIImpl<DX12ResourceState>
	{
		struct FrameData
		{
			UINT64 mFence = 0;
			std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
		};

	private:
		blDX12ResourceCacheGlobalInterface mResourceCacheGlobalInterface;

		std::unique_ptr<blMeshRenderComponent> mMeshRenderComponent;
		std::unique_ptr<blMeshInstancedRenderComponent> mMeshInstancedRenderComponent;
		std::unique_ptr<blCompositeMeshRenderComponent> mCompositeMeshRenderComponent;
		std::shared_ptr<blDX12Imgui> mDX12ImguiRenderComponent;
		std::unique_ptr<blCompositeMeshRenderWithPassConstantsRenderComponent> mCompositeMeshRenderWithPassConstantsRenderComponent;

		std::vector<blRenderComponentBase*> mRenderComponents;

		std::set<blResourceId> mDirtyResources;
	private:
		std::shared_ptr<blGlobalRenderData> mGlobalRenderDataPtr;
		std::vector<std::shared_ptr<blDX12ResourceCacheBase>> mResourceCaches;
		std::shared_ptr<Core::blWindow> mWindow;
		std::map<blSceneResource, blRenderSceneContext> mSceneContextMap;
		std::shared_ptr<blCommandList> mCommandList;
		std::shared_ptr<blFence> mFence;
		std::array<FrameData, Constants::FrameResourceCount> mFrameData;
		UINT64 mCurrentFence;
	public:
		virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		virtual void InitializeInternal() override;
		virtual void UpdateInternal(const Metrics::blTime& time) override;
		virtual void DrawMesh(const RenderMeshData& renderItem, const blSceneResourcePtr scene) override;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene) override;
		virtual void DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData, const blSceneResourcePtr scene) override;
		virtual void DrawCompositeMeshWithPass(const RenderCompositeMeshDataWithPassConstants& renderData, const blSceneResourcePtr scene) override;

		virtual void MarkResourceDirty(const blResourceId resourceId) override;
	public:
		blDX12(std::shared_ptr<Core::blWindow> window);
		~blDX12();
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;

		void StartFrameInternal() override;
		void EndFrameInternal() override;
	private:
		void FlushCommandQueue();
	};
}