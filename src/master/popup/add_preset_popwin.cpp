#include "add_preset_popwin.h"
#include "src/imgui_notify.h"

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
    if(value){
        preset_name = "";
    }
}

void AddPresetPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        bool updated = ImGui::InputText("Preset Name", &preset_name);
        if(ImGui::IsItemHovered()){
            ImGui::SetItemTooltip("Enter preset name");
        }
        if (ImGui::Button("Confirm")) {
            save_preset();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void AddPresetPopup::save_preset(){
    json data = json::object();
    if(preset_name.size() == 0){
        ImGuiToast toast(ImGuiToastType_Error, 3000);
        toast.set_title("Preset Save Failed");
        toast.set_content("You need to enter preset name");
        ImGui::InsertNotification(toast);
        return;
    }
    int32_t result = master->add_preset(preset_name, data);
    if(result == 0){
        ImGuiToast toast(ImGuiToastType_Success, 3000);
        toast.set_title("Preset Save Success");
        toast.set_content("Preset name: %s", preset_name.c_str());
        ImGui::InsertNotification(toast);
        state->update_preset();
        trigger(false);
    }else if(result == 2){
        ImGuiToast toast(ImGuiToastType_Error, 3000);
        toast.set_title("Preset Save Failed");
        toast.set_content("Name already used: %s", preset_name.c_str());
        ImGui::InsertNotification(toast);
    }
}