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
			std::unique_ptr<blCommandListAllocator> mCommandListAllocator;
		};

	protected:
		blGlobalRenderData* mGlobalRenderData;
		std::unique_ptr<blCommandList> mCommandList;
		std::array<FrameData, Constants::FrameResourceCount> mFrameData;
	public:
		blRenderComponentBase() = default;
		virtual ~blRenderComponentBase() = default;

		blRenderComponentBase(blGlobalRenderData* globalRenderDataPtr,
			std::wstring name = L"blRenderComponentBase")
			: mGlobalRenderData(globalRenderDataPtr)
		{
			for (FrameData& frameData : mFrameData)
			{
				frameData.mCommandListAllocator = std::make_unique<blCommandListAllocator>(globalRenderDataPtr->device.get(), L"RenderComponent");
			}

			FrameData& frameData = mFrameData[mGlobalRenderData->globalRenderFrameContext->GetCurrentFrameResource()];
			mCommandList = std::make_unique<blCommandList>(frameData.mCommandListAllocator.get(), name);
		}

		virtual void Initialize() 
		{
			mCommandList->Close();
		};

		virtual void StartFrame() 
		{
			FrameData& frameData = mFrameData[mGlobalRenderData->globalRenderFrameContext->GetCurrentFrameResource()];
			blCommandListAllocator* commandListAllocator = frameData.mCommandListAllocator.get();
			commandListAllocator->GetAllocatorPtr().Get()->Reset();
			mCommandList->Reset(commandListAllocator);
			mCommandList->RSSetViewports(1, &mGlobalRenderData->viewPort);
			mCommandList->RSSetScissorRects(1, &mGlobalRenderData->scissorRect);
		};

		virtual void EndFrame()
		{
			mCommandList->Close();
		};

		blCommandList* GetCommandList()
		{
			return mCommandList.get();
		}
	};

	template<typename TRenderData>
	class blRenderComponent : public blRenderComponentBase
	{
	public:
		blRenderComponent() = default;
		virtual ~blRenderComponent() = default;

		blRenderComponent(blGlobalRenderData* globalRenderDataPtr,
			std::wstring name = L"blRenderComponent")
			: blRenderComponentBase(globalRenderDataPtr, name)
		{
		}

		virtual void Render(const TRenderData& renderData, const blSceneResourcePtr scene) = 0;

	};
}