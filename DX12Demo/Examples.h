#pragma once

#include "AbstractExample.h"

namespace BoulderLeaf::Graphics::DX12
{
	enum class Example
	{
		None = -1,
		Box,
		LineStrip,
		IndexBuffer,
		ImGui,
		Shapes,
		Count
	};

	//I tried returning std::shared_ptr<Example>, but the compiler complained. Could not figure out why. 
	//"cannot instantiate abstract class"
	AbstractExample* GetNewExample(std::shared_ptr <DX12> dx12, const Example example);
	Example Next(Example current);
	Example Prev(Example current);
	void DrawImgui();
}