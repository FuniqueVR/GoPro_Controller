#include "studio.popwin.h"

StudioPopup::StudioPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Studio##Popup";
}

StudioPopup::~StudioPopup(){
    
}

void StudioPopup::trigger(bool value){
    BasePopWindow::trigger(value);
}

void StudioPopup::render(){
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 display_size = io.DisplaySize;
    ImVec2 unit = ImVec2(display_size.x / 12.0f, display_size.y / 12.0f);

    ImGui::SetNextWindowPos(ImVec2(unit.x * 0.5F, unit.y * 0.5F), wp_cond);
    ImGui::SetNextWindowSize(ImVec2(unit.x * 11.0F, unit.y * 11.0F), wp_cond);

    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        ImGui::EndPopup();
    }
}