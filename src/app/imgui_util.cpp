#include "app/imgui_util.h"

#include <cstring>

#include "imgui.h"

namespace OrderbookApp {
void SetHorizontalCentered(const char* text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth   = ImGui::CalcTextSize(text).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
}
}
