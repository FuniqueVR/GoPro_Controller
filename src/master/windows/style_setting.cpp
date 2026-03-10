#include "style_setting.h"
#include <imgui.h>
#include "../imgui_helper.h"

#define GET_DATA_COLOR4(x, y) x["colors"][#y]=json::object();\
store_vec4(x["colors"][#y],colors[y]);\

#define SET_DATA_COLOR4(x, y) fetch_vec4(x["colors"][#y],colors[y]);\

void store_vec4(json& j, ImVec4& value){
    j["x"] = value.x;
    j["y"] = value.y;
    j["z"] = value.z;
    j["w"] = value.w;
}
void fetch_vec4(json& j, ImVec4& value){
    value.x = j["x"];
    value.y = j["y"];
    value.z = j["z"];
    value.w = j["w"];
}

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
    json buffer = json::object();

    buffer["colors"] = json::object();
    GET_DATA_COLOR4(buffer, ImGuiCol_WindowBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_ChildBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_PopupBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_Border);
    GET_DATA_COLOR4(buffer, ImGuiCol_BorderShadow);
    GET_DATA_COLOR4(buffer, ImGuiCol_FrameBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_FrameBgHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_FrameBgActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_TitleBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_TitleBgActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_TitleBgCollapsed);
    GET_DATA_COLOR4(buffer, ImGuiCol_MenuBarBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_ScrollbarBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_ScrollbarGrab);
    GET_DATA_COLOR4(buffer, ImGuiCol_ScrollbarGrabHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_ScrollbarGrabActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_CheckMark);
    GET_DATA_COLOR4(buffer, ImGuiCol_SliderGrab);
    GET_DATA_COLOR4(buffer, ImGuiCol_SliderGrabActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_Button);
    GET_DATA_COLOR4(buffer, ImGuiCol_ButtonHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_ButtonActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_Header);
    GET_DATA_COLOR4(buffer, ImGuiCol_HeaderHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_HeaderActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_Separator);
    GET_DATA_COLOR4(buffer, ImGuiCol_SeparatorHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_SeparatorActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_ResizeGrip);
    GET_DATA_COLOR4(buffer, ImGuiCol_ResizeGripHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_ResizeGripActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_Tab);
    GET_DATA_COLOR4(buffer, ImGuiCol_TabHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_TabActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_TabUnfocused);
    GET_DATA_COLOR4(buffer, ImGuiCol_TabUnfocusedActive);
    GET_DATA_COLOR4(buffer, ImGuiCol_DockingPreview);
    GET_DATA_COLOR4(buffer, ImGuiCol_DockingEmptyBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_PlotLines);
    GET_DATA_COLOR4(buffer, ImGuiCol_PlotLinesHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_PlotHistogram);
    GET_DATA_COLOR4(buffer, ImGuiCol_PlotHistogramHovered);
    GET_DATA_COLOR4(buffer, ImGuiCol_TableHeaderBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_TableBorderStrong);
    GET_DATA_COLOR4(buffer, ImGuiCol_TableBorderLight);
    GET_DATA_COLOR4(buffer, ImGuiCol_TableRowBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_TableRowBgAlt);
    GET_DATA_COLOR4(buffer, ImGuiCol_TextSelectedBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_DragDropTarget);
    GET_DATA_COLOR4(buffer, ImGuiCol_NavHighlight);
    GET_DATA_COLOR4(buffer, ImGuiCol_NavWindowingHighlight);
    GET_DATA_COLOR4(buffer, ImGuiCol_NavWindowingDimBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_ModalWindowDimBg);
    GET_DATA_COLOR4(buffer, ImGuiCol_Text);
    GET_DATA_COLOR4(buffer, ImGuiCol_TextDisabled);

    return buffer;
}

void StyleSetting::set_window_data(json data) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    SET_DATA_COLOR4(data, ImGuiCol_WindowBg);
    SET_DATA_COLOR4(data, ImGuiCol_ChildBg);
    SET_DATA_COLOR4(data, ImGuiCol_PopupBg);
    SET_DATA_COLOR4(data, ImGuiCol_Border);
    SET_DATA_COLOR4(data, ImGuiCol_BorderShadow);
    SET_DATA_COLOR4(data, ImGuiCol_FrameBg);
    SET_DATA_COLOR4(data, ImGuiCol_FrameBgHovered);
    SET_DATA_COLOR4(data, ImGuiCol_FrameBgActive);
    SET_DATA_COLOR4(data, ImGuiCol_TitleBg);
    SET_DATA_COLOR4(data, ImGuiCol_TitleBgActive);
    SET_DATA_COLOR4(data, ImGuiCol_TitleBgCollapsed);
    SET_DATA_COLOR4(data, ImGuiCol_MenuBarBg);
    SET_DATA_COLOR4(data, ImGuiCol_ScrollbarBg);
    SET_DATA_COLOR4(data, ImGuiCol_ScrollbarGrab);
    SET_DATA_COLOR4(data, ImGuiCol_ScrollbarGrabHovered);
    SET_DATA_COLOR4(data, ImGuiCol_ScrollbarGrabActive);
    SET_DATA_COLOR4(data, ImGuiCol_CheckMark);
    SET_DATA_COLOR4(data, ImGuiCol_SliderGrab);
    SET_DATA_COLOR4(data, ImGuiCol_SliderGrabActive);
    SET_DATA_COLOR4(data, ImGuiCol_Button);
    SET_DATA_COLOR4(data, ImGuiCol_ButtonHovered);
    SET_DATA_COLOR4(data, ImGuiCol_ButtonActive);
    SET_DATA_COLOR4(data, ImGuiCol_Header);
    SET_DATA_COLOR4(data, ImGuiCol_HeaderHovered);
    SET_DATA_COLOR4(data, ImGuiCol_HeaderActive);
    SET_DATA_COLOR4(data, ImGuiCol_Separator);
    SET_DATA_COLOR4(data, ImGuiCol_SeparatorHovered);
    SET_DATA_COLOR4(data, ImGuiCol_SeparatorActive);
    SET_DATA_COLOR4(data, ImGuiCol_ResizeGrip);
    SET_DATA_COLOR4(data, ImGuiCol_ResizeGripHovered);
    SET_DATA_COLOR4(data, ImGuiCol_ResizeGripActive);
    SET_DATA_COLOR4(data, ImGuiCol_Tab);
    SET_DATA_COLOR4(data, ImGuiCol_TabHovered);
    SET_DATA_COLOR4(data, ImGuiCol_TabActive);
    SET_DATA_COLOR4(data, ImGuiCol_TabUnfocused);
    SET_DATA_COLOR4(data, ImGuiCol_TabUnfocusedActive);
    SET_DATA_COLOR4(data, ImGuiCol_DockingPreview);
    SET_DATA_COLOR4(data, ImGuiCol_DockingEmptyBg);
    SET_DATA_COLOR4(data, ImGuiCol_PlotLines);
    SET_DATA_COLOR4(data, ImGuiCol_PlotLinesHovered);
    SET_DATA_COLOR4(data, ImGuiCol_PlotHistogram);
    SET_DATA_COLOR4(data, ImGuiCol_PlotHistogramHovered);
    SET_DATA_COLOR4(data, ImGuiCol_TableHeaderBg);
    SET_DATA_COLOR4(data, ImGuiCol_TableBorderStrong);
    SET_DATA_COLOR4(data, ImGuiCol_TableBorderLight);
    SET_DATA_COLOR4(data, ImGuiCol_TableRowBg);
    SET_DATA_COLOR4(data, ImGuiCol_TableRowBgAlt);
    SET_DATA_COLOR4(data, ImGuiCol_TextSelectedBg);
    SET_DATA_COLOR4(data, ImGuiCol_DragDropTarget);
    SET_DATA_COLOR4(data, ImGuiCol_NavHighlight);
    SET_DATA_COLOR4(data, ImGuiCol_NavWindowingHighlight);
    SET_DATA_COLOR4(data, ImGuiCol_NavWindowingDimBg);
    SET_DATA_COLOR4(data, ImGuiCol_ModalWindowDimBg);
    SET_DATA_COLOR4(data, ImGuiCol_Text);
    SET_DATA_COLOR4(data, ImGuiCol_TextDisabled);
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
            bool changed = false;
            if(ImGui::BeginTabItem("Colors##style_win")){
                changed = changed | render_colors();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Fields##style_win")){
                changed = render_fields();
                ImGui::EndTabItem();
            }
            if(changed){
                update_style();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

bool StyleSetting::render_colors(){
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

bool StyleSetting::render_fields(){
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    bool changed = false;
    changed = changed | ImGui::InputFloat("Font Size Base##style_field", &style.FontSizeBase);
    changed = changed | ImGui::InputFloat("Font Scale Main##style_field", &style.FontScaleMain);
    changed = changed | ImGui::InputFloat2("Window Padding##style_field", (float*)&style.WindowPadding);
    changed = changed | ImGui::InputFloat("Window Padding##style_field", &style.WindowRounding);
    return changed;
}

bool StyleSetting::update_style(){
    state->update_server();
}
