#pragma once

#include "../../ext/imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../ext/imgui/imgui_internal.h"

#include "../../ext/imgui/imgui_tricks.hpp"



namespace custom {
    bool tab(const char* icon, const char* label, bool selected);
    bool subtab(const char* label, bool selected);
    void begin_child(const char* name, ImVec2 size);
    void end_child();
    bool collapse_button(bool collapsed);
    void ImRotateStart();
    ImVec2 ImRotationCenter();
   
    bool tab(const char* label, bool selected);
}


