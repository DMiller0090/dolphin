#include <string>

#include "imgui.h"
#include "Core/API/Gui.h"  // adjust if your header path differs

namespace API::Gui
{
    void DrawText(ImVec2 pos, unsigned int color, std::string text)
    {
        ImGui::GetForegroundDrawList()->AddText(
            pos,
            color,
            text.c_str()
        );
    }
}
