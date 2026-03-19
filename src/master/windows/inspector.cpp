/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "inspector.h"
#include <iostream>
#include <string>
#include <cstdlib>      // For std::getenv
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

std::filesystem::path get_home_directory() {
    const char* home_dir = nullptr;

#if defined(_WIN32) || defined(_WIN64)
    // Windows: Check USERPROFILE or HOMEDRIVE/HOMEPATH
    home_dir = std::getenv("USERPROFILE");
    if (!home_dir) {
        const char* drive = std::getenv("HOMEDRIVE");
        const char* path = std::getenv("HOMEPATH");
        if (drive && path) {
            return std::filesystem::path(drive) / path;
        }
    }
#else
    // Unix-like systems (Linux, macOS, etc.): Check HOME
    home_dir = std::getenv("HOME");
#endif

    if (home_dir) {
        return std::filesystem::path(home_dir);
    } else {
        // Fallback or throw an error if the home directory cannot be found
        throw std::runtime_error("Could not find home directory");
    }
}

InspectorWindow::InspectorWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Inspector";
    setting_list_ordered = std::vector<int32_t>(GOPRO_SETTING_SIZE);
    status_list_ordered = std::vector<int32_t>(GOPRO_STATUS_SIZE);
    reset_setting_order();
    reset_status_order();
}

InspectorWindow::~InspectorWindow(){
    
}

json InspectorWindow::get_window_data() {
    json data = json::object();
    data["current_download_location"] = state->current_download_location;
    data["setting_order"] = json::array();
    data["status_order"] = json::array();
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        data["setting_order"].push_back(setting_list_ordered[i]);
    }
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        data["status_order"].push_back(status_list_ordered[i]);
    }
    return data;
}

void InspectorWindow::set_window_data(json data) {
    if(data["current_download_location"].is_string()){
        state->current_download_location = data["current_download_location"].get<std::string>();
    }
    if(data["setting_order"].is_array() && data["setting_order"].size() == GOPRO_SETTING_SIZE){
        for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
            if(data["setting_order"].at(i).is_number_integer()){
                setting_list_ordered[i] = data["setting_order"].at(i).get<int32_t>();
            }
        }
    }
    if(data["status_order"].is_array() && data["status_order"].size() == GOPRO_STATUS_SIZE){
        for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
            if(data["status_order"].at(i).is_number_integer()){
                status_list_ordered[i] = data["status_order"].at(i).get<int32_t>();
            }
        }
    }
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
        ImGui::Text("Name: %s, IP: %s", state->current_camera_name.c_str(), state->current_camera_item.c_str());

        ImGui::BeginDisabled(should_disabled);

        if(ImGui::Button("Reset Setting Order")){
            reset_setting_order();
            state->update_server();
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset Status Order")){
            reset_status_order();
            state->update_server();
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
    int move_from = -1, move_to = -1;
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        int32_t id = setting_list_ordered[i];
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
        
        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
        //src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;
        if(ImGui::BeginDragDropSource(src_flags)){
            if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                ImGui::Text("Moving \"%s\"", name.c_str());
            ImGui::SetDragDropPayload("INSPECTOR_SETTING", &i, sizeof(int));
            ImGui::EndDragDropSource();
        }

        if(ImGui::BeginDragDropTarget()){
            ImGuiDragDropFlags target_flags = 0;
            target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
            target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INSPECTOR_SETTING", target_flags))
            {
                move_from = *(const int*)payload->Data;
                move_to = i;
            }
            ImGui::EndDragDropTarget();
        }
    }
    if (move_from != -1 && move_to != -1)
    {
        const int32_t tmp = setting_list_ordered[move_from];
        setting_list_ordered[move_from] = setting_list_ordered[move_to];
        setting_list_ordered[move_to] = tmp;
        //ImGui::SetDragDropPayload("INSPECTOR_SETTING", &move_to, sizeof(int));
        state->update_server();
    }
}

void InspectorWindow::draw_status(){
    int move_from = -1, move_to = -1;
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        float x = ImGui::GetCursorPosX();
        int32_t id = status_list_ordered[i];
        const int32_t type = GET_STATUS_TYPE_BY_ID(id);
        std::string name = GET_STATUS_NAME_BY_ID(id);
        if (name.size() == 0) continue;

        name += "##InspectorTitle_status";
        std::string pp = "##InspectorTitle_status_option_" + std::string(GET_STATUS_NAME_BY_ID(id));

        if(type == (int32_t)CAMERA_STATUS_TYPE::OPTION){
            size_t size = GET_STATUS_SIZE_BY_ID(id);
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t select_index = state->current_status_items[std::to_string(id)].get<int32_t>();
            const char** select_string_list = GET_STATUS_STRING_BY_ID(id);
            std::string select_value_display = select_string_list[select_index];
            pp = std::to_string(select_index) + " " + select_value_display + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::BOOLEAN){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t value = state->current_status_items[std::to_string(id)].get<int32_t>();
            std::string display = "";
            if(value == 1) display = "true";
            else display = "false";
            pp = display + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::INT){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int32_t value = state->current_status_items[std::to_string(id)].get<int32_t>();
            pp = std::to_string(value) + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::LONG){
            if (!state->current_status_items[std::to_string(id)].is_number()) continue;
            int64_t value = state->current_status_items[std::to_string(id)].get<int64_t>();
            pp = std::to_string(value) + pp;
        }else if (type == (int32_t)CAMERA_STATUS_TYPE::STRING){
            if (!state->current_status_items[std::to_string(id)].is_string()) continue;
            std::string value = state->current_status_items[std::to_string(id)].get<std::string>();
            pp = value + pp;
        }else {
            std::cout << "Type is unknown: " << type << std::endl;
            continue;   
        }

        ImGui::Selectable(pp.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;
        if(ImGui::BeginDragDropSource(src_flags)){
            if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
                ImGui::Text("Moving \"%s\"", name.c_str());
            ImGui::SetDragDropPayload("INSPECTOR_STATUS", &i, sizeof(int));
            ImGui::EndDragDropSource();
        }

        if(ImGui::BeginDragDropTarget()){
            ImGuiDragDropFlags target_flags = 0;
            target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
            target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INSPECTOR_STATUS", target_flags))
            {
                move_from = *(const int*)payload->Data;
                move_to = i;
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(x + ImGui::CalcItemWidth() + ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::TextUnformatted(GET_STATUS_NAME_BY_ID(id));
    }
    if (move_from != -1 && move_to != -1)
    {
        const int32_t tmp = status_list_ordered[move_from];
        status_list_ordered[move_from] = status_list_ordered[move_to];
        status_list_ordered[move_to] = tmp;
        state->update_server();
    }
}

void InspectorWindow::draw_media(){
    int32_t camera_ip = master->findCamera(state->current_camera_item);
    if(ImGui::InputText("Media Download", &state->current_download_location)){
        state->update_server();
    }
    if(ImGui::Button("All Download")){
        master->download_last_media(state->current_download_location);
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Download all exist camera instances");
    ImGui::SameLine();
    if(ImGui::Button("Single Download")){
        
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Download current select camera instance");

    if(ImGui::Button("Open Home")){
        std::system((std::string("open \"") + get_home_directory().string() + std::string("\"")).c_str());
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Open file explorer for home directory");
    ImGui::SameLine();
    if(ImGui::Button("Open Select Path")){
        if(fs::exists(state->current_download_location)){
            std::system((std::string("open \"") + state->current_download_location + std::string("\"")).c_str());
        }
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Open file explorer for path select directory");
    if(camera_ip >= 0){
        std::shared_ptr<CameraInfo> t = master->getCameras()[camera_ip];
        ImGui::LabelText("Server", "%s", t->server.c_str());
        ImGui::LabelText("Last Media", "%s", t->last_media.c_str());
    }
}

void InspectorWindow::reset_setting_order(){
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        setting_list_ordered[i] = GOPRO_SETTING_IDS[i];
    }
}

void InspectorWindow::reset_status_order(){
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        status_list_ordered[i] = GOPRO_STATUS_IDS[i];
    }
}
