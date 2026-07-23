#include <DemoImgui.h>
#include <blCore.h>
#include <algorithm>
#include <blDemoImguiDropdown.h>

namespace
{
    static BoulderLeaf::Imgui::blDemoImguiDropdown demoDropdown(
        std::vector<std::string>{"Box","Cylinder","Geosphere", "Shapes", "Shapes With Pass", "Terrain", "Material & Lighting"},
        "DemoSelector",
        "Example"
    );
}

namespace BoulderLeaf::Imgui
{
    int GetCurrentSelection()
    {
        return demoDropdown.GetSelected();
    }

    void SetCurrentSelection(int value)
    {
        demoDropdown.SetSelected(value);
    }

    void DrawDemoImgui()
    {
        demoDropdown.DrawWindow();
    }
}