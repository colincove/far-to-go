#pragma once

#include <blGlobalRenderData.h>
#include <memory>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blRenderComponentBase
	{
	protected:
		std::shared_ptr<blGlobalRenderData> mGlobalRenderData;
		std::shared_ptr<blCommandList> mCommandList;
		std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
	public:
		blRenderComponentBase() = default;
		virtual ~blRenderComponentBase() = default;

		blRenderComponentBase(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr,
			std::wstring name = L"blRenderComponentBase")
			: mGlobalRenderData(globalRenderDataPtr)
		{
			mCommandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataPtr->device, L"RenderComponent");
			mCommandList = std::make_shared<blCommandList>(mCommandListAllocator, name);
		}

		virtual void Initialize() 
		{
			mCommandList->Close();
		};

		virtual void StartFrame() 
		{
			mCommandListAllocator->GetAllocatorPtr().Get()->Reset();
			mCommandList->Reset(mCommandListAllocator);
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

		std::shared_ptr<blCommandListAllocator> GetCommandListAllocator()
		{
			return mCommandListAllocator;
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