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

std::vector<int32_t> Decode_Schedule_Time(int32_t value){
    std::vector<int32_t> a = std::vector<int32_t>();
    a.push_back(0); a.push_back(0); a.push_back(0);

    if(value % 4 == 0) return std::vector<int32_t>();
    if((value - 1) % 4 == 0 && value < 3073) a[0] = 1;
    else if((value - 1) % 4 == 0 && value >= 3073) a[0] = 2;
    else if((value + 1) % 4 == 0) a[0] = 3;

    if(a[0] == 3){ // 24H
        for(int32_t i = 0; i < 24; i++){ // 256 gap
            int32_t starter = 3 + (256 * i);
            if(value >= starter && value <= starter + 236){
                a[1] = i;
                a[2] = (value - starter) / 4;
                break;
            }
        }
    }else{ // 12H
        for(int32_t i = 0; i < 12; i++){ // 256 gap
            int32_t starter = -1;
            if(a[0] == 2){ // PM
                starter = 3073 + (256 * i);
            }else{ // AM
                starter = 1 + (256 * i);
            }

            if(value >= starter && value <= starter + 236){
                a[1] = i;
                a[2] = (value - starter) / 4;
                break;
            }
        }
    }

    return a;
}

int32_t Encode_Schedule_Time(std::vector<int32_t> value){
    if(value.size() == 0) return 0;
    int32_t a = 0;
    if(value.at(0) == 0) return 0;

    if(value.at(0) == 1) a = 1; // AM
    if(value.at(0) == 2) a = 3073; // PM
    if(value.at(0) == 3) a = 3; // 24H

    a += value.at(1) * 256;
    a += value.at(2) * 4;
    return a;
}

InspectorWindow::InspectorWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Inspector";
    system_list_ordered = std::vector<int32_t>(GOPRO_SYSTEM_SETTING_SIZE);
    video_setting_list_ordered = std::vector<int32_t>(GOPRO_VIDEO_SETTING_SIZE);
    photo_setting_list_ordered = std::vector<int32_t>(GOPRO_PHOTO_SETTING_SIZE);
    video_protune_list_ordered = std::vector<int32_t>(GOPRO_VIDEO_PROTUNE_SETTING_SIZE);
    photo_protune_list_ordered = std::vector<int32_t>(GOPRO_PHOTO_PROTUNE_SETTING_SIZE);
    status_list_ordered = std::vector<int32_t>(GOPRO_STATUS_SIZE);
    reset_setting_order();
    reset_status_order();
}

InspectorWindow::~InspectorWindow(){
    
}

json InspectorWindow::get_window_data() {
    json data = json::object();
    data["put_finish"] = put_finish;
    data["create_date_folder"] = create_date_folder;
    data["current_download_location"] = state->current_download_location;
    data["setting_order"] = json::object();
    data["status_order"] = json::array();
    data["setting_order"]["system_list_ordered"] = json::array();
    data["setting_order"]["video_setting_list_ordered"] = json::array();
    data["setting_order"]["photo_setting_list_ordered"] = json::array();
    data["setting_order"]["video_protune_list_ordered"] = json::array();
    data["setting_order"]["photo_protune_list_ordered"] = json::array();
    for(int32_t i = 0; i < system_list_ordered.size(); i++){
        data["setting_order"]["system_list_ordered"].push_back(system_list_ordered[i]);
    }
    for(int32_t i = 0; i < video_setting_list_ordered.size(); i++){
        data["setting_order"]["video_setting_list_ordered"].push_back(video_setting_list_ordered[i]);
    }
    for(int32_t i = 0; i < photo_setting_list_ordered.size(); i++){
        data["setting_order"]["photo_setting_list_ordered"].push_back(photo_setting_list_ordered[i]);
    }
    for(int32_t i = 0; i < video_protune_list_ordered.size(); i++){
        data["setting_order"]["video_protune_list_ordered"].push_back(video_protune_list_ordered[i]);
    }
    for(int32_t i = 0; i < photo_protune_list_ordered.size(); i++){
        data["setting_order"]["photo_protune_list_ordered"].push_back(photo_protune_list_ordered[i]);
    }
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        data["status_order"].push_back(status_list_ordered[i]);
    }
    return data;
}

void InspectorWindow::set_window_data(json data) {
    if(data["put_finish"].is_boolean()){
        put_finish = data["put_finish"].get<bool>();
    }
    if(data["create_date_folder"].is_boolean()){
        create_date_folder = data["create_date_folder"].get<bool>();
    }
    if(data["current_download_location"].is_string()){
        state->current_download_location = data["current_download_location"].get<std::string>();
    }
    if(data["setting_order"].is_object()){
        if(data["setting_order"]["system_list_ordered"].is_array() && data["setting_order"]["system_list_ordered"].size() == system_list_ordered.size()){
            for(int32_t i = 0; i < system_list_ordered.size(); i++){
                if(data["setting_order"]["system_list_ordered"].at(i).is_number_integer()){
                    system_list_ordered[i] = data["setting_order"]["system_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["video_setting_list_ordered"].is_array() && data["setting_order"]["video_setting_list_ordered"].size() == video_setting_list_ordered.size()){
            for(int32_t i = 0; i < system_list_ordered.size(); i++){
                if(data["setting_order"]["video_setting_list_ordered"].at(i).is_number_integer()){
                    video_setting_list_ordered[i] = data["setting_order"]["video_setting_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["photo_setting_list_ordered"].is_array() && data["setting_order"]["photo_setting_list_ordered"].size() == photo_setting_list_ordered.size()){
            for(int32_t i = 0; i < photo_setting_list_ordered.size(); i++){
                if(data["setting_order"]["photo_setting_list_ordered"].at(i).is_number_integer()){
                    photo_setting_list_ordered[i] = data["setting_order"]["photo_setting_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["video_protune_list_ordered"].is_array() && data["setting_order"]["video_protune_list_ordered"].size() == video_protune_list_ordered.size()){
            for(int32_t i = 0; i < video_protune_list_ordered.size(); i++){
                if(data["setting_order"]["video_protune_list_ordered"].at(i).is_number_integer()){
                    video_protune_list_ordered[i] = data["setting_order"]["video_protune_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["photo_protune_list_ordered"].is_array() && data["setting_order"]["photo_protune_list_ordered"].size() == photo_protune_list_ordered.size()){
            for(int32_t i = 0; i < photo_protune_list_ordered.size(); i++){
                if(data["setting_order"]["photo_protune_list_ordered"].at(i).is_number_integer()){
                    photo_protune_list_ordered[i] = data["setting_order"]["photo_protune_list_ordered"].at(i).get<int32_t>();
                }
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

        draw_header();
        ImGui::BeginDisabled(should_disabled);

        ImGui::Separator();

        if(ImGui::BeginTabBar("Inspector_Bar##Top")){
            if(ImGui::BeginTabItem("Setting##Inspector_Bar_Item")){
                if(ImGui::Button("Reset Setting Order")){
                    reset_setting_order();
                    state->update_server();
                }
                if(ImGui::BeginTabBar("Inspector_Bar##Second")){
                    if(ImGui::BeginTabItem("System##Inspector_Bar_Item")){
                        draw_system();
                        ImGui::EndTabItem();
                    }
                    if(ImGui::BeginTabItem("Setting##Inspector_Bar_Item")){
                        draw_setting();
                        ImGui::EndTabItem();
                    }
                    if(ImGui::BeginTabItem("Protune##Inspector_Bar_Item")){
                        draw_protune();
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Status##Inspector_Bar_Item")){
                if(ImGui::Button("Reset Status Order")){
                    reset_status_order();
                    state->update_server();
                }
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

void InspectorWindow::draw_header(){
    ImGui::InputText("Camera Name", &state->current_camera_name);
    if(ImGui::Button("Rename Camera")){
        master->command_with_value("rename", state->current_camera_item, state->current_camera_name);
    }
    ImGui::Text("Name: %s, IP: %s", state->current_camera_name.c_str(), state->current_camera_item.c_str());
}

void InspectorWindow::draw_system(){
    _draw_setting(system_list_ordered);
}

void InspectorWindow::draw_setting(){
    if (state->current_status_items[std::to_string(PRESET_ID)].is_number()) {
        int32_t preset = state->current_status_items[std::to_string(PRESET_ID)].get<int32_t>();
        if(preset == 0){
            _draw_setting(video_setting_list_ordered);
        }else{
            _draw_setting(photo_setting_list_ordered);
        }
    }
}

void InspectorWindow::draw_protune(){
    if (state->current_status_items[std::to_string(PRESET_ID)].is_number()) {
        int32_t preset = state->current_status_items[std::to_string(PRESET_ID)].get<int32_t>();
        if(preset == 0){
            _draw_setting(video_protune_list_ordered);
        }else{
            _draw_setting(photo_protune_list_ordered);       
        }
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
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 size = ImGui::GetWindowSize();
    ImVec2 button_size = ImVec2(size.x / 2.0F - style.ItemSpacing.x, 0);

    int32_t camera_ip = master->findCamera(state->current_camera_item);
    if(ImGui::InputText("Media Download", &state->current_download_location)){
        state->update_server();
    }
    if(ImGui::Button("All Download", button_size)){
        std::string buffer = state->current_download_location;
        while(buffer.size() > 0 && buffer.at(buffer.size() - 1) == '/'){
            buffer.pop_back();
        }
        if(master->directoryExists(buffer)){
            if(create_date_folder){
                std::string date = getCurrentDateTimeString();
                buffer.append("/" + date);
            }
            fs::create_directories(buffer);
            master->download_last_media(buffer, put_finish);
        }
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Download all exist camera instances");
    ImGui::SameLine();
    if(ImGui::Button("Single Download", button_size)){
        
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Download current select camera instance");

    if(ImGui::Button("Open Home", button_size)){
        std::system((std::string("open \"") + get_home_directory().string() + std::string("\"")).c_str());
    }
    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Open file explorer for home directory");
    ImGui::SameLine();
    if(ImGui::Button("Open Select Path", button_size)){
        if(fs::exists(state->current_download_location)){
            std::system((std::string("open \"") + state->current_download_location + std::string("\"")).c_str());
        }
    }

    if(ImGui::Checkbox("Create Date Folder", &create_date_folder)){
        state->update_server();
    }
    if(ImGui::Checkbox("Added Finish File", &put_finish)){
        state->update_server();
    }

    if(ImGui::IsItemHovered()) ImGui::SetTooltip("Open file explorer for path select directory");
    if(camera_ip >= 0){
        std::shared_ptr<CameraInfo> t = master->getCameras()[camera_ip];
        ImGui::LabelText("Server", "%s", t->server.c_str());
        ImGui::LabelText("Last Media", "%s", t->last_media.c_str());
    }
}

void InspectorWindow::draw_command_local(){

}

void InspectorWindow::draw_command_global(){
    
}

void InspectorWindow::reset_setting_order(){
    for(int32_t i = 0; i < system_list_ordered.size(); i++){
        system_list_ordered[i] = GOPRO_SYSTEM_SETTING_IDS[i];
    }
    for(int32_t i = 0; i < video_setting_list_ordered.size(); i++){
        video_setting_list_ordered[i] = GOPRO_VIDEO_SETTING_IDS[i];
    }
    for(int32_t i = 0; i < photo_setting_list_ordered.size(); i++){
        photo_setting_list_ordered[i] = GOPRO_PHOTO_SETTING_IDS[i];
    }
    for(int32_t i = 0; i < video_protune_list_ordered.size(); i++){
        video_protune_list_ordered[i] = GOPRO_VIDEO_PROTUNE_SETTING_IDS[i];
    }
    for(int32_t i = 0; i < photo_protune_list_ordered.size(); i++){
        photo_protune_list_ordered[i] = GOPRO_PHOTO_PROTUNE_SETTING_IDS[i];
    }
}

void InspectorWindow::reset_status_order(){
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        status_list_ordered[i] = GOPRO_STATUS_IDS[i];
    }
}

void InspectorWindow::_draw_setting(std::vector<int32_t>& ordered){
    int32_t move_from = -1, move_to = -1;
    int32_t model_enum = _get_current_model();
    for(int32_t i = 0; i < ordered.size(); i++){
        int32_t id = ordered[i];
        std::string name = GET_SETTING_NAME_BY_ID(id);
        size_t size = GET_SETTING_SIZE_BY_ID(id);
        int32_t ava = GET_SETTING_AVA_BY_ID(id);
        if((model_enum&ava) == 0){
            continue;
        }
        if (!state->current_setting_items[std::to_string(id)].is_number()) {
            continue;
        }
        if (name.size() == 0) {
            continue;
        }
        if(!conditional_filter(model_enum, id)){
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
        const int32_t* support_id = GET_SETTING_SUPPORT_BY_ID(id);

        if(ImGui::BeginCombo(name.c_str(), select_string)){
            for (int n = 0; n < size; n++)
            {
                int32_t supp = support_id[n];
                if((model_enum&supp) == 0){
                    continue;
                }
                if(!conditional_filter_option(model_enum, id, n)){
                    continue;
                }

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
        const int32_t tmp = ordered[move_from];
        ordered[move_from] = ordered[move_to];
        ordered[move_to] = tmp;
        //ImGui::SetDragDropPayload("INSPECTOR_SETTING", &move_to, sizeof(int));
        state->update_server();
    }
}

int32_t InspectorWindow::_get_current_model(){
    if(state->current_hw_items["model_name"].is_string()){
        std::string model_name = state->current_hw_items["model_name"].get<std::string>();
        if(model_name == "MAX 2") return MODEL_MAX2;
        else if(model_name == "HERO13 Black") return MODEL_13;
        else if(model_name == "HERO12 Black") return MODEL_12;
        else if(model_name == "HERO11 Black") return MODEL_11_BLACK;
        else if(model_name == "HERO11 Black Mini") return MODEL_11;
        else if(model_name == "HERO10 Black") return MODEL_10;
        else if(model_name == "HERO9 Black") return MODEL_9;
    }
    else return 0;
}

bool InspectorWindow::conditional_filter(int32_t mymodel, int32_t setting_id){
    if(setting_id == SHUTTER_SPEED_VIDEO_ID){
        int32_t profile = 0;
        if(state->current_setting_items[std::to_string(PROFILES_ID)].is_number()){
            profile = state->current_setting_items[std::to_string(PROFILES_ID)].get<int32_t>();
        }
        if(profile == 1) return false;
    }
    return true;
}

bool InspectorWindow::conditional_filter_option(int32_t mymodel, int32_t setting_id, int32_t value_index){
    if(setting_id == VIDEO_RESOLUTION_ID){
        int32_t aspect = 0;
        if(state->current_setting_items[std::to_string(VIDEO_ASPECT_RATIO_ID)].is_number()){
            aspect = state->current_setting_items[std::to_string(VIDEO_ASPECT_RATIO_ID)].get<int32_t>();
        }
        int32_t res_id = VIDEO_RESOLUTION_VALUE[value_index];
        int32_t aspect_id = VIDEO_ASPECT_RATIO_VALUE[aspect];
        if(aspect_id == 0){ // "4:3"
            if(res_id != 6 &&
            res_id != 18 && res_id != 25 && res_id != 27 &&
            res_id != 111 && res_id != 112 && res_id != 113){
                return false;
            }
        }
        else if(aspect_id == 1){ // "16:9"
            if(res_id != 1 && res_id != 4 && res_id != 9 &&
            res_id != 100){
                return false;
            }
        }
        else if(aspect_id == 3){ // "8:7"
            if(res_id != 26 && res_id != 28 && res_id != 107 &&
            res_id != 108){
                return false;
            }
        }
        else if(aspect_id == 4){ // "9:16"
            if(res_id != 109 && res_id != 110){
                return false;
            }
        }
        else if(aspect_id == 5){ // "21:9"
            if(res_id != 35 && res_id != 36){
                return false;
            }
        }
        else if(aspect_id == 6){ // "1:1"
            if(res_id != 37){
                return false;
            }
        }
    }
    else if(setting_id == SHUTTER_SPEED_VIDEO_ID){
        if(mymodel&(ANTI_FLICKER_V2_AVA) > 0){
            int32_t antif = 0;
            if(state->current_setting_items[std::to_string(ANTI_FLICKER_V2_ID)].is_number()){
                antif = state->current_setting_items[std::to_string(ANTI_FLICKER_V2_ID)].get<int32_t>();
            }
            if(antif == 1){
                if(value_index != 0 && value_index != 51 && value_index != 50 &&
                value_index != 30 && value_index != 49 && value_index != 29 &&
                value_index != 48 && value_index != 28 && value_index != 47 &&
                value_index != 21 && value_index != 46 && value_index != 17 &&
                value_index != 45 && value_index != 12 && value_index != 44){
                    return false;
                }
            }else{
                if(value_index != 0 && value_index != 56 && value_index != 55 &&
                value_index != 31 && value_index != 54 && value_index != 24 &&
                value_index != 53 && value_index != 23 && value_index != 27 &&
                value_index != 22 && value_index != 20 && value_index != 18 &&
                value_index != 15 && value_index != 13 && value_index != 10 &&
                value_index != 8){
                    return false;
                }
            }
        }
    }
    return true;
}
