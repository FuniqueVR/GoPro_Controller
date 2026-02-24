/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "inspector.h"

InspectorWindow::InspectorWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Inspector";
}

InspectorWindow::~InspectorWindow(){
    
}

void InspectorWindow::render(){
    ImGui::Begin("Inspector", &enable, w_flag);
    {
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        int32_t camera_ip = master->findCamera(state->current_camera_item);
        bool should_disabled = state->current_camera_item.size() < 10 || camera_ip == -1 || !state->current_setting_items_bind;
        

        ImGui::InputText("Camera Name", &state->current_camera_name);
        if(ImGui::Button("Rename Camera")){
            master->command_with_value("rename", state->current_camera_item, state->current_camera_name);
        }

        ImGui::BeginDisabled(should_disabled);
        ImGui::InputText("Media Download", &state->current_download_location);
        if(ImGui::Button("All Download")){
            master->download_last_media(state->current_download_location);
        }
        ImGui::SameLine();
        if(ImGui::Button("Single Download")){
            
        }
        if(camera_ip >= 0){
            std::shared_ptr<CameraInfo> t = master->getCameras()[camera_ip];
            ImGui::LabelText("Server", "%s", t->server.c_str());
            ImGui::LabelText("Last Media", "%s", t->last_media.c_str());
        }
        ImGui::Separator();

        if(ImGui::BeginTabBar("Inspector_Bar")){
            if(ImGui::BeginTabItem("Setting##Inspector_Bar_Item")){
                draw_setting();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Status##Inspector_Bar_Item")){
                draw_status();
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Media##Inspector_Bar_Item")){
                draw_media();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        
        ImGui::EndDisabled();
    }
    ImGui::End();
}

void InspectorWindow::draw_setting(){
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        int32_t id = GOPRO_SETTING_IDS[i];
        std::string name = GET_SETTING_NAME_BY_ID(id);
        size_t size = GET_SETTING_SIZE_BY_ID(id);
        if (!state->current_setting_items[std::to_string(id)].is_number()) {
            continue;
        }
        if (name.size() == 0) {
            continue;
        }

        name += "##InspectorTitle";
        int32_t select_index = state->current_setting_items[std::to_string(id)].get<int32_t>();
        const char** select_string_list = GET_SETTING_STRING_BY_ID(id);
        if(select_string_list == nullptr) {
            std::cerr << "Inspector: select_string_list == nullptr" << std::endl;
            continue;
        }
        if(select_index >= size) {
            std::cerr << "Inspector: select_index >= size..." << id << "..." << select_index << "..." << size << std::endl;
            continue;
        }
        const char* select_string = select_string_list[select_index];
        if(select_string == nullptr) continue;

        const int32_t* values_id = GET_SETTING_VALUE_BY_ID(id);

        if(ImGui::BeginCombo(name.c_str(), select_string)){
            for (int n = 0; n < size; n++)
            {
                std::string option = GET_SETTING_STRING_BY_ID(id)[n];
                if(option.size() == 0) continue;
                bool is_selected = (state->current_setting_items[std::to_string(id)] == n); // You can store your selection however you want, outside or inside your objects
                option += ("##InspectorOption_" + name); 
                if (ImGui::Selectable(option.c_str(), is_selected))
                {
                    state->current_setting_items[std::to_string(id)] = n; // Change index
                    master->apply(state->current_camera_item, id, values_id[n]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }
    }
}

void InspectorWindow::draw_status(){
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        int32_t id = GOPRO_STATUS_IDS[i];
        const int32_t type = GET_STATUS_TYPE_BY_ID(id);
        std::string name = GET_STATUS_NAME_BY_ID(id);
        if (name.size() == 0) continue;

        name += "##InspectorTitle_status";

        if(type == CAMERA_STATUS_TYPE::OPTION){
            size_t size = GET_STATUS_SIZE_BY_ID(id);
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t select_index = state->current_setting_items[std::to_string(id)].get<int32_t>();
            const char** select_string_list = GET_STATUS_STRING_BY_ID(id);
            std::string select_value_display = select_string_list[select_index];
            ImGui::LabelText(name.c_str(), "%s", select_value_display.c_str());
        }else if (type == CAMERA_STATUS_TYPE::BOOLEAN){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t value = state->current_setting_items[std::to_string(id)].get<int32_t>();
            std::string display = "";
            if(value == 1) display = "true";
            else display = "false";
            ImGui::LabelText(name.c_str(), "%s", display.c_str());
        }else if (type == CAMERA_STATUS_TYPE::INT){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t value = state->current_setting_items[std::to_string(id)].get<int32_t>();
            ImGui::LabelText(name.c_str(), "%d", value);
        }else if (type == CAMERA_STATUS_TYPE::LONG){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int64_t value = state->current_setting_items[std::to_string(id)].get<int64_t>();
            ImGui::LabelText(name.c_str(), "%ld", value);
        }else if (type == CAMERA_STATUS_TYPE::STRING){
            if (!state->current_status_items[std::to_string(id)].is_string()) continue;
            std::string value = state->current_setting_items[std::to_string(id)].get<std::string>();
            ImGui::LabelText(name.c_str(), "%s", value.c_str());
        }else {
            continue;   
        }
    }
}

void InspectorWindow::draw_media(){

}
