#pragma once

#include <blGlobalRenderData.h>
#include <memory>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blRenderComponentBase
	{
		struct FrameData
		{
			std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
		};

	protected:
		std::shared_ptr<blGlobalRenderData> mGlobalRenderData;
		std::shared_ptr<blCommandList> mCommandList;
		std::array<FrameData, Constants::FrameResourceCount> mFrameData;
	public:
		blRenderComponentBase() = default;
		virtual ~blRenderComponentBase() = default;

		blRenderComponentBase(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr,
			std::wstring name = L"blRenderComponentBase")
			: mGlobalRenderData(globalRenderDataPtr)
		{
			for (FrameData& frameData : mFrameData)
			{
				frameData.mCommandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataPtr->device, L"RenderComponent");
			}

			FrameData& frameData = mFrameData[mGlobalRenderData->globalRenderFrameContext->GetCurrentFrameResource()];
			std::shared_ptr<blCommandListAllocator> commandListAllocator = frameData.mCommandListAllocator;
			mCommandList = std::make_shared<blCommandList>(commandListAllocator, name);
		}

		virtual void Initialize() 
		{
			mCommandList->Close();
		};

		virtual void StartFrame() 
		{
			FrameData& frameData = mFrameData[mGlobalRenderData->globalRenderFrameContext->GetCurrentFrameResource()];
			std::shared_ptr<blCommandListAllocator> commandListAllocator = frameData.mCommandListAllocator;
			commandListAllocator->GetAllocatorPtr().Get()->Reset();
			mCommandList->Reset(commandListAllocator);
			mCommandList->RSSetViewports(1, &mGlobalRenderData->viewPort);
			mCommandList->RSSetScissorRects(1, &mGlobalRenderData->scissorRect);
		};

		virtual void EndFrame()
		{
			mCommandList->Close();
		};

		std::shared_ptr<blCommandList> GetCommandList()
		{
			return mCommandList;
		}
	};

	template<typename TRenderData>
	class blRenderComponent : public blRenderComponentBase
	{
	public:
		blRenderComponent() = default;
		virtual ~blRenderComponent() = default;

		blRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr,
			std::wstring name = L"blRenderComponent")
			: blRenderComponentBase(globalRenderDataPtr, name)
		{
		}

		virtual void Render(const TRenderData& renderData, const blSceneResourcePtr scene) = 0;

	};
}