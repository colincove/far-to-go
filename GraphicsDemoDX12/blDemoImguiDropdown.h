#pragma once

#include <imgui.h>
#include <string>
#include <vector>

namespace BoulderLeaf::Imgui
{
	class blDemoImguiDropdown
	{
	private:
		std::vector<std::string> mItems;
		int mSelectedIndex;
		ImVec2 mPosition;
		ImVec2 mSize;
		std::string mId;
		std::string mWindowName;
		std::string mLabel;
		bool mIsOpen = true;

	public:
		blDemoImguiDropdown() = delete;
		blDemoImguiDropdown(const std::vector<std::string>& items, const std::string& windowName = "Dropdown", const std::string& label = "Select");

		void SetPosition(const ImVec2& pos);
		void SetSize(const ImVec2& size);
		int GetSelected() const;
		void SetSelected(int idx);

		// Draw the dropdown as a combo inside the current ImGui window
		void DrawInline();

		// Draw the dropdown inside its own small window at mPosition/mSize
		void DrawWindow();
	};
}
