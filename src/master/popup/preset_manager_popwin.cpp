#include "preset_manager_popwin.h"

PresetManagerPopup::PresetManagerPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Preset Manager##Popup";
}


PresetManagerPopup::~PresetManagerPopup(){
    
}

void PresetManagerPopup::trigger(bool value){
    BasePopWindow::trigger(value);
}

void PresetManagerPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        
        if (ImGui::Button("Confirm")) {
            
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}