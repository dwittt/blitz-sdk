#include "customs.h"

#include <string>
#include <map>

using namespace ImGui;


struct tab_anim
{
    int hovered_text_anim;
    int active_text_anim;
};

int rotation_start_index;
void custom::ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 custom::ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

// tab
bool custom::tab(const char* label, bool selected) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x + 5, pos.y + 20));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 2.f, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_anim> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, {0, 0} });
        it_anim = anim.find(id);
    }

    if (hovered)
        it_anim->second.hovered_text_anim += 8 * (1.f - ImGui::GetIO().DeltaTime);
    else
        it_anim->second.hovered_text_anim -= 8 * (1.f - ImGui::GetIO().DeltaTime);

    if (it_anim->second.hovered_text_anim > 30)
        it_anim->second.hovered_text_anim = 30;
    else if (it_anim->second.hovered_text_anim < 0)
        it_anim->second.hovered_text_anim = 0;

    if (selected) {
        it_anim->second.active_text_anim += 16 * (1.f - ImGui::GetIO().DeltaTime);
    }
    else {
        it_anim->second.active_text_anim -= 16 * (1.f - ImGui::GetIO().DeltaTime);
    }

    if (it_anim->second.active_text_anim > 155)
        it_anim->second.active_text_anim = 155;
    else if (it_anim->second.active_text_anim < 0)
        it_anim->second.active_text_anim = 0;

    //PushFont(general);
    window->DrawList->AddText(ImVec2((rect.Min.x + rect.Max.x) / 2.f - (label_size.x / 2.f), (rect.Min.y + rect.Max.y) / 2.f - (label_size.y / 2.f)), ImColor(255, 255, 255, (160 + it_anim->second.hovered_text_anim + it_anim->second.active_text_anim)), label);
    //PopFont();

    return pressed;
}

bool custom::subtab(const char* label, bool selected) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiID id = window->GetID(label);

    ImVec2 p = window->DC.CursorPos;
    ImVec2 size({ CalcTextSize(label, 0, true).x + 10, window->Size.y });
    ImRect bb(p, { p.x + size.x, p.y + size.y });

    ItemSize(bb);
    ItemAdd(bb, id);

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    float anim = ImTricks::Animations::FastFloatLerp(std::string(label).append("subtab.anim"), selected, 0.f, 1.f, 0.04f);
    auto col = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_TextDisabled), GetColorU32(ImGuiCol_Scheme), anim);

    window->DrawList->AddRectFilled({ bb.Min.x, bb.Max.y - 2 }, bb.Max, GetColorU32(ImGuiCol_Scheme, anim), 2, ImDrawFlags_RoundCornersTop);

    PushStyleColor(ImGuiCol_Text, col.Value);
    RenderText(bb.GetCenter() - CalcTextSize(label, 0, true) / 2, label);
    PopStyleColor();

    return pressed;
}

void custom::begin_child(const char* name, ImVec2 size) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImVec2 p = window->DC.CursorPos;

    BeginChild(std::string(name).append("main").c_str(), size, true, ImGuiWindowFlags_NoScrollbar);
    GetWindowDrawList()->AddText(GetIO().Fonts->Fonts[2], GetIO().Fonts->Fonts[0]->FontSize, { p.x + 15, p.y + 15 }, GetColorU32(ImGuiCol_Text), name);
    SetCursorPosY(50);
    BeginChild(name, { size.x, size.y == 0 ? size.y : size.y - 50 }, false, ImGuiWindowFlags_NoBackground);
    SetCursorPosX(15);

    BeginGroup();

    PushStyleVar(ImGuiStyleVar_ItemSpacing, { 10, 12 });
}

void custom::end_child() {
    PopStyleVar();
    EndGroup();
    EndChild();
    EndChild();
}

