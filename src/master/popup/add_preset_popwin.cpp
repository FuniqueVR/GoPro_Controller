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
        
        ImGui::EndPopup();
    }
}