#include "add_preset_popwin.h"

AddPresetPopup::AddPresetPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Add Preset##Popup";
}


AddPresetPopup::~AddPresetPopup(){
    
}

void AddPresetPopup::trigger(bool value){
    BasePopWindow::trigger(value);
}

void AddPresetPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        bool updated = ImGui::InputText("Preset Name", &preset_name);
        if(ImGui::IsItemHovered()){
            ImGui::SetItemTooltip("Enter preset name");
        }
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", msg.c_str());

        if (ImGui::Button("Confirm")) {
            
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}