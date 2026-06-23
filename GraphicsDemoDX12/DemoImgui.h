#pragma once

#include <imgui.h>

namespace BoulderLeaf::Imgui
{
	void DrawDemoImgui();
	int GetCurrentSelection();
	void SetCurrentSelection(int value);
}