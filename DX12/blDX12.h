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
#include <RenderComponents/blMeshInstancedV2RenderComponent.h>
#include <RenderComponents/blCompositeMeshRenderComponent.h>
#include <RenderComponents/blCompositeMeshRenderWithPassConstantsRenderComponent.h>
#include <blDX12Imgui.h>
#include <ResourceCache/blDX12ResourceCacheInterface.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blDX12 : public blGraphicsAPIImpl
	{
		struct FrameData
		{
			UINT64 mFence = 0;
			std::unique_ptr<blCommandListAllocator> mCommandListAllocator;
		};

	private:
		std::unique_ptr<blDX12ResourceCacheGlobalInterface> mResourceCacheGlobalInterface;

		std::unique_ptr<blMeshRenderComponent> mMeshRenderComponent;
		std::unique_ptr<blMeshInstancedRenderComponent> mMeshInstancedRenderComponent;
	std::unique_ptr<blMeshInstancedV2RenderComponent> mMeshInstancedV2RenderComponent;
		std::unique_ptr<blCompositeMeshRenderComponent> mCompositeMeshRenderComponent;
		std::shared_ptr<blDX12Imgui> mDX12ImguiRenderComponent;
		std::unique_ptr<blCompositeMeshRenderWithPassConstantsRenderComponent> mCompositeMeshRenderWithPassConstantsRenderComponent;

		std::vector<blRenderComponentBase*> mRenderComponents;
	private:
		blGlobalRenderData mGlobalRenderData;
		Core::blWindow* mWindow;
		std::unique_ptr<blCommandList> mCommandList;
		std::unique_ptr<blFence> mFence;
		std::array<FrameData, Constants::FrameResourceCount> mFrameData;
		UINT64 mCurrentFence;
	public:
		virtual LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		virtual void InitializeInternal() override;
		virtual void UpdateInternal(const Metrics::blTime& time) override;
		virtual void DrawMesh(const RenderMeshData& renderItem) override;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData) override;
	virtual void DrawMeshInstancedV2(const RenderMeshDataInstancedV2& renderData) override;
		virtual void DrawCompositeMeshInstanced(const RenderCompositeMeshDataInstanced& renderData) override;
		virtual void DrawCompositeMeshWithPass(const RenderCompositeMeshDataWithPassConstants& renderData) override;
	public:
		blDX12(Core::blWindow* window);
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