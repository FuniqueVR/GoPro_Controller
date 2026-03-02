#include "start_webcam_popwin.h"

StartWebcamPopup::StartWebcamPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Start Webcam##Popup";
}

StartWebcamPopup::~StartWebcamPopup(){
    
}

void StartWebcamPopup::render(){
    if(ImGui::BeginPopupModal("Start Webcam##Popup", NULL, wp_flag)){
        bool updated = false;
        updated = ImGui::InputText("Server IP", &server_ip_buf);
        updated = ImGui::InputText("Port Start", &port_buf);
        updated = ImGui::Checkbox("Use TS", &ts_buf);
        if(ImGui::BeginCombo("Res", res_string_buf.c_str())){
            for(int32_t n = 0; n < WEBCAM_START_RES_SIZE; n++){
                std::string option = WEBCAM_START_RES_STRING[n];
                if(option.size() == 0) continue;
                bool is_selected = (res_string_buf == option); // You can store your selection however you want, outside or inside your objects
                std::string option_r = option + ("##WebcamOption_" + std::string(WEBCAM_START_RES_NAME)); 
                if (ImGui::Selectable(option_r.c_str(), is_selected))
                {
                    updated = true;
                    res_buf = n; // Change index
                    res_string_buf = option;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); 
            }
            ImGui::EndCombo();
        }
        if(ImGui::BeginCombo("Fov", fov_string_buf.c_str())){
            for(int32_t n = 0; n < WEBCAM_START_FOV_SIZE; n++){
                std::string option = WEBCAM_START_FOV_STRING[n];
                if(option.size() == 0) continue;
                bool is_selected = (fov_string_buf == option); // You can store your selection however you want, outside or inside your objects
                std::string option_r = option + ("##WebcamOption_" + std::string(WEBCAM_START_FOV_NAME)); 
                if (ImGui::Selectable(option.c_str(), is_selected))
                {
                    updated = true;
                    fov_buf = n; // Change index
                    fov_string_buf = option;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); 
            }
            ImGui::EndCombo();
        }
        
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", error.c_str());
        if(updated){
            error.clear();
        }
        if(ImGui::Button("Confirm")){
            
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")){
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}