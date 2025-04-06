#pragma once

#include "AbstractExample.h"

namespace BoulderLeaf::Graphics::DX12
{
	//I tried returning std::shared_ptr<Example>, but the compiler complained. Could not figure out why. 
	//"cannot instantiate abstract class"
	AbstractExample* GetNewExample(std::shared_ptr <DX12> dx12, const int index);
	void DrawImgui();
}