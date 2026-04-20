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
            bool pass = true;
            int32_t index = master->findServer(server_ip_buf);
            if(index == -1){
                error = "Server does not exist.";
                pass = false;
            }
            if(!all_servers[index]->connected){
                error = "Server is disconnected.";
                pass = false;
            }
            if(master->findCamera(GetRemoteIPBySerial(camera_serial_buf)) != -1){
                error = "Camera already added.";
                pass = false;
            }
            if(camera_serial_buf.size() != 3){
                error = "Serial must be 3 number";
                pass = false;
            }

            if(pass){
                master->command_only(server_ip_buf, "add", std::string(camera_serial_buf));
                trigger(false);
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}