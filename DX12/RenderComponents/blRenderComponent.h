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
	public:
		blRenderComponentBase() = default;
		virtual ~blRenderComponentBase() = default;

		blRenderComponentBase(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr)
			: mGlobalRenderDataPtr(globalRenderDataPtr)
		{
		}

		virtual void StartFrame() {};
	};

	template<typename TRenderData>
	class blRenderComponent : public blRenderComponentBase
	{
	public:
		blRenderComponent() = default;
		virtual ~blRenderComponent() = default;

		blRenderComponent(std::shared_ptr<blGlobalRenderData> globalRenderDataPtr)
			: blRenderComponentBase(globalRenderDataPtr)
		{
		}

		virtual void Render(const TRenderData& renderData, const blSceneResourcePtr scene) = 0;
	};
}