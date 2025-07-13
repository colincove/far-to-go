#pragma once
#include <blTypes.h>
#include <blRenderScene.h>

namespace BoulderLeaf::Graphics
{

	enum class RenderItemType : long
	{
		Mesh,
		Count
	};


	template<typename TData, RenderItemType type>
	class RenderItem
	{
	private: 
		RenderItemType mType;
		TData mData;
	public:
		constexpr static byte Size = sizeof(TData);

		RenderItem()
			: mType(type)
		{
		}

		void Submit(blRenderScene& scene, const TData& data) {}
	};
}