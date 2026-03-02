#include "scan_camera_popwin.h"

ScanCameraPopup::ScanCameraPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Scan Camera##Popup";
}

ScanCameraPopup::~ScanCameraPopup(){
    
}

void ScanCameraPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        bool updated = false;
        updated = ImGui::InputText("Server IP", &server_ip_buf);
        ImGui::Text("You can leave it empty for broadcast to all websocket server");
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", error.c_str());
        if(updated){
            error.clear();
        }
        if (ImGui::Button("Confirm")) {
            bool pass = true;
            if(master->findServer(server_ip_buf) == -1 && sizeof(server_ip_buf) == 0){
                error = "Server does not exist.";
                pass = false;
            }

            if(pass){
                if(sizeof(server_ip_buf) == 0){
                    master->command_only("scan");
                }else{
                    master->command_only(server_ip_buf, "scan", "");
                }
            }
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();               
    }
}