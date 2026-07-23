#include <blDemoImguiDropdown.h>

namespace BoulderLeaf::Imgui
{
	blDemoImguiDropdown::blDemoImguiDropdown(const std::vector<std::string>& items, const std::string& windowName, const std::string& label)
		: mItems(items), mSelectedIndex(0), mPosition(ImVec2(0, 0)), mSize(ImVec2(300, 100)), mId(windowName), mWindowName(windowName), mLabel(label)
	{
	}

	void blDemoImguiDropdown::SetPosition(const ImVec2& pos)
	{
		mPosition = pos;
	}

	void blDemoImguiDropdown::SetSize(const ImVec2& size)
	{
		mSize = size;
	}

	int blDemoImguiDropdown::GetSelected() const
	{
		return mSelectedIndex;
	}

	void blDemoImguiDropdown::SetSelected(int idx)
	{
		if (idx >= 0 && idx < static_cast<int>(mItems.size()))
			mSelectedIndex = idx;
	}

	void blDemoImguiDropdown::DrawInline()
	{
		std::vector<const char*> cstrs;
		cstrs.reserve(mItems.size());
		for (const auto& s : mItems) cstrs.push_back(s.c_str());
		ImGui::Combo(mLabel.c_str(), &mSelectedIndex, cstrs.data(), static_cast<int>(cstrs.size()));
	}

	void blDemoImguiDropdown::DrawWindow()
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoBackground;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoResize;
		windowFlags |= ImGuiWindowFlags_NoCollapse;

		ImGui::SetNextWindowPos(mPosition);
		ImGui::SetNextWindowSize(mSize);

		if (ImGui::Begin(mWindowName.c_str(), &mIsOpen, windowFlags))
		{
			DrawInline();
		}
		ImGui::End();
	}
}
