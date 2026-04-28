#include "add_camera_popwin.h"

AddCameraPopup::AddCameraPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Add Camera##Popup";
}


AddCameraPopup::~AddCameraPopup(){
    
}

void AddCameraPopup::trigger(bool value){
    BasePopWindow::trigger(value);
    if(value){
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        const std::vector<std::shared_ptr<ServerConnection>>& refs = master->getServers();
        if(refs.size() > 0){
            server_ip_buf = refs[0]->ip;
        }
    }
}

void AddCameraPopup::render(){
    if(ImGui::BeginPopupModal(title.c_str(), NULL, wp_flag)){
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        const std::vector<std::shared_ptr<ServerConnection>>& all_servers = master->getServers();
        bool updated = false;
        if(ImGui::BeginCombo("Server IP", server_ip_buf.c_str())){
            for (int n = 0; n < all_servers.size(); n++)
            {
                std::string option = all_servers.at(n)->ip;
                bool is_selected = all_servers[n]->ip == server_ip_buf;
                option += "##PopupServerOption"; 
                ImGui::BeginDisabled(!all_servers[n]->connected);
                if (ImGui::Selectable(option.c_str(), is_selected))
                {
                    server_ip_buf = all_servers.at(n)->ip;
                }
                ImGui::EndDisabled();
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }
        updated = ImGui::InputText("Camera Serial", &camera_serial_buf);
        if(ImGui::IsItemHovered()){
            ImGui::SetItemTooltip("Enter the camera last 3 serial number\nIt will automatically generate ip address for you.");
        }
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", error.c_str());
        if(updated){
            error.clear();
        }

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