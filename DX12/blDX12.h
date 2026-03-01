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
#include <blRenderScene.h>
#include <blRenderSceneContext.h>
#include <blGlobalRenderData.h>
#include <blGraphicsAPIImpl.h>
#include <RenderComponents/blMeshRenderComponent.h>
#include <RenderComponents/blMeshInstancedRenderComponent.h>

namespace BoulderLeaf::Graphics::DX12
{
	struct DX12ResourceState : public ResourceState
	{
	};

	class blDX12 : public blGraphicsAPIImpl<DX12ResourceState>
	{
	private:
		blMeshRenderComponent mMeshRenderComponent;
		blMeshInstancedRenderComponent mMeshInstancedRenderComponent;

		blRenderComponentBase* mRenderComponents[2] = 
		{
			&mMeshRenderComponent , 
			&mMeshInstancedRenderComponent 
		};
	private:
		std::shared_ptr<blGlobalRenderData> mGlobalRenderDataPtr;
		std::shared_ptr<Core::blWindow> mWindow;
		std::map<blSceneResource, blRenderSceneContext> mSceneContextMap;
		UINT64 mCurrentFence;
	public:
		void StartFrame() override;
		void EndFrame() override;
	public:
		blDX12(std::shared_ptr<Core::blWindow> window);
		~blDX12();
	protected:
		void OnWindowMessage(MSG msg) override;
		void OnResize() override;
		virtual void DrawMesh(const RenderMeshData& renderItem, const blSceneResourcePtr scene) override;
		virtual void DrawMeshInstanced(const RenderMeshDataInstanced& renderData, const blSceneResourcePtr scene) override;
		virtual void InitializeGroup(const blRenderGroupId& group) override;

		void GroupStartFrame(blRenderGroupId group);
		void GroupEndFrame(blRenderGroupId group);
	private:
		void FlushCommandQueue();
	};
}