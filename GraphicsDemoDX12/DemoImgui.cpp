#include <DemoImgui.h>
#include <blCore.h>
#include <algorithm>

namespace
{
    static int item_current = 0;
}

namespace BoulderLeaf::Imgui
{
    int GetCurrentSelection()
    {
        return item_current;
    }

    void SetCurrentSelection(int value)
    {
        item_current = value;
    }

    void DrawDemoImgui()
    {
        static bool isOpen = true;

        ImGuiWindowFlags windowFlags = 0;
        windowFlags |= ImGuiWindowFlags_NoBackground;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoCollapse;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(std::min(300.0f, viewport->WorkSize.x), viewport->WorkSize.y));

        if (!ImGui::Begin("Graphics DX12 Demo", &isOpen, windowFlags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        const char* items[] = { "Box","Cylinder","Geosphere", "Shapes", "Shapes With Pass"};
        ImGui::Combo("Example", &item_current, items, C_ARRAY_COUNT(items));
        ImGui::End();
    }
}