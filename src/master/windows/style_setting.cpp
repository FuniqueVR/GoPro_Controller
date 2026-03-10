#include "style_setting.h"
#include <imgui.h>
#include "../imgui_helper.h"

StyleSetting::StyleSetting(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Style Setting";
}

StyleSetting::~StyleSetting(){
    
}

json StyleSetting::get_window_data() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    return json::object();
}

void StyleSetting::set_window_data(json data) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
}

void StyleSetting::render(){
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    ImGui::Begin("Style Setting", &enable, w_flag);
    {
        if(ImGui::BeginCombo("Theme##style_themes", "")){
            if(ImGui::Selectable("Dark Theme")){
                ImGui::StyleColorsDark();
            }
            if(ImGui::Selectable("Light Theme")){
                ImGui::StyleColorsLight();
            }
            if(ImGui::Selectable("Mocha Theme")){
                setup_catppuccin_mocha_theme();
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();
        
        if(ImGui::BeginTabBar("Style Fields##style_win")){
            if(ImGui::BeginTabItem("Colors##style_win")){
                redner_colors();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Fields##style_win")){
                redner_fields();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::SliderFloat("Alpha##style_field", &style.Alpha, 0.01f, 1);
        ImGui::InputFloat("Font Size Base##style_field", &style.FontSizeBase);
        ImGui::InputFloat("Font Scale Main##style_field", &style.FontScaleMain);
        ImGui::InputFloat2("Window Padding##style_field", (float*)&style.WindowPadding);
        ImGui::InputFloat("Window Padding##style_field", &style.WindowRounding);
    }
    ImGui::End();
}

bool StyleSetting::redner_colors(){
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    bool changed = false;
    changed = changed | ImGui::ColorEdit4("ImGuiCol_WindowBg##style_color_edit", (float*)&colors[ImGuiCol_WindowBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ChildBg##style_color_edit", (float*)&colors[ImGuiCol_ChildBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_PopupBg##style_color_edit", (float*)&colors[ImGuiCol_PopupBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_Border##style_color_edit", (float*)&colors[ImGuiCol_Border]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_BorderShadow##style_color_edit", (float*)&colors[ImGuiCol_BorderShadow]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_FrameBg##style_color_edit", (float*)&colors[ImGuiCol_FrameBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_FrameBgHovered##style_color_edit", (float*)&colors[ImGuiCol_FrameBgHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_FrameBgActive##style_color_edit", (float*)&colors[ImGuiCol_FrameBgActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TitleBg##style_color_edit", (float*)&colors[ImGuiCol_TitleBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TitleBgActive##style_color_edit", (float*)&colors[ImGuiCol_TitleBgActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TitleBgCollapsed##style_color_edit", (float*)&colors[ImGuiCol_TitleBgCollapsed]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_MenuBarBg##style_color_edit", (float*)&colors[ImGuiCol_MenuBarBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ScrollbarBg##style_color_edit", (float*)&colors[ImGuiCol_ScrollbarBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ScrollbarGrab##style_color_edit", (float*)&colors[ImGuiCol_ScrollbarGrab]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ScrollbarGrabHovered##style_color_edit", (float*)&colors[ImGuiCol_ScrollbarGrabHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ScrollbarGrabActive##style_color_edit", (float*)&colors[ImGuiCol_ScrollbarGrabActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_CheckMark##style_color_edit", (float*)&colors[ImGuiCol_CheckMark]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_SliderGrab##style_color_edit", (float*)&colors[ImGuiCol_SliderGrab]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_SliderGrabActive##style_color_edit", (float*)&colors[ImGuiCol_SliderGrabActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_Button##style_color_edit", (float*)&colors[ImGuiCol_Button]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ButtonHovered##style_color_edit", (float*)&colors[ImGuiCol_ButtonHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ButtonActive##style_color_edit", (float*)&colors[ImGuiCol_ButtonActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_Header##style_color_edit", (float*)&colors[ImGuiCol_Header]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_HeaderHovered##style_color_edit", (float*)&colors[ImGuiCol_HeaderHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_HeaderActive##style_color_edit", (float*)&colors[ImGuiCol_HeaderActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_Separator##style_color_edit", (float*)&colors[ImGuiCol_Separator]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_SeparatorHovered##style_color_edit", (float*)&colors[ImGuiCol_SeparatorHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_SeparatorActive##style_color_edit", (float*)&colors[ImGuiCol_SeparatorActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ResizeGrip##style_color_edit", (float*)&colors[ImGuiCol_ResizeGrip]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ResizeGripHovered##style_color_edit", (float*)&colors[ImGuiCol_ResizeGripHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ResizeGripActive##style_color_edit", (float*)&colors[ImGuiCol_ResizeGripActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_Tab##style_color_edit", (float*)&colors[ImGuiCol_Tab]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TabHovered##style_color_edit", (float*)&colors[ImGuiCol_TabHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TabActive##style_color_edit", (float*)&colors[ImGuiCol_TabActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TabUnfocused##style_color_edit", (float*)&colors[ImGuiCol_TabUnfocused]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TabUnfocusedActive##style_color_edit", (float*)&colors[ImGuiCol_TabUnfocusedActive]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_DockingPreview##style_color_edit", (float*)&colors[ImGuiCol_DockingPreview]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_DockingEmptyBg##style_color_edit", (float*)&colors[ImGuiCol_DockingEmptyBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_PlotLines##style_color_edit", (float*)&colors[ImGuiCol_PlotLines]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_PlotLinesHovered##style_color_edit", (float*)&colors[ImGuiCol_PlotLinesHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_PlotHistogram##style_color_edit", (float*)&colors[ImGuiCol_PlotHistogram]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_PlotHistogramHovered##style_color_edit", (float*)&colors[ImGuiCol_PlotHistogramHovered]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TableHeaderBg##style_color_edit", (float*)&colors[ImGuiCol_TableHeaderBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TableBorderStrong##style_color_edit", (float*)&colors[ImGuiCol_TableBorderStrong]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TableBorderLight##style_color_edit", (float*)&colors[ImGuiCol_TableBorderLight]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TableRowBg##style_color_edit", (float*)&colors[ImGuiCol_TableRowBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TableRowBgAlt##style_color_edit", (float*)&colors[ImGuiCol_TableRowBgAlt]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TextSelectedBg##style_color_edit", (float*)&colors[ImGuiCol_TextSelectedBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_DragDropTarget##style_color_edit", (float*)&colors[ImGuiCol_DragDropTarget]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_NavHighlight##style_color_edit", (float*)&colors[ImGuiCol_NavHighlight]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_NavWindowingHighlight##style_color_edit", (float*)&colors[ImGuiCol_NavWindowingHighlight]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_NavWindowingDimBg##style_color_edit", (float*)&colors[ImGuiCol_NavWindowingDimBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_ModalWindowDimBg##style_color_edit", (float*)&colors[ImGuiCol_ModalWindowDimBg]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_Text##style_color_edit", (float*)&colors[ImGuiCol_Text]);
    changed = changed | ImGui::ColorEdit4("ImGuiCol_TextDisabled##style_color_edit", (float*)&colors[ImGuiCol_TextDisabled]);
    return changed;
}

bool StyleSetting::redner_fields(){

}

bool StyleSetting::update_style(){
    ImGuiStyle& style = ImGui::GetStyle();
}
