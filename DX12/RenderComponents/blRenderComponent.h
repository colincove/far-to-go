#pragma once

#include <blGlobalRenderData.h>
#include <memory>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics::DX12
{
	class blRenderComponentBase
	{
	protected:
		std::shared_ptr<blGlobalRenderData> mGlobalRenderDataPtr;
		std::shared_ptr<blCommandList> mCommandList;
		std::shared_ptr<blCommandListAllocator> mCommandListAllocator;
	public:
		blRenderComponentBase() = default;
		virtual ~blRenderComponentBase() = default;

		blRenderComponentBase(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr,
			std::wstring name = L"blRenderComponentBase")
			: mGlobalRenderDataPtr(globalRenderDataPtr)
		{

			mCommandListAllocator = std::make_shared<blCommandListAllocator>(globalRenderDataPtr->device, L"RenderComponent");
			mCommandList = std::make_shared<blCommandList>(mCommandListAllocator, name);

			DX12_API_CALL(mCommandList->GetCommandListPtr()->Close());
			mCommandListAllocator->Reset();
			DX12_API_CALL(mCommandList->GetCommandListPtr()->Reset(mCommandListAllocator->GetAllocatorPtr().Get(), nullptr));
		}

		virtual void StartFrame() 
		{
			ComPtr<ID3D12GraphicsCommandList> commandList = mCommandList->GetCommandListPtr();
			mCommandListAllocator->GetAllocatorPtr().Get()->Reset();
			commandList->Reset(mCommandListAllocator->GetAllocatorPtr().Get(), nullptr);
		};
		virtual void EndFrame() {};
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