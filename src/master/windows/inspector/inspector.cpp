#include "../inspector.h"


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
            for(int32_t i = 0; i < data["setting_order"]["system_list_ordered"].size(); i++){
                if(data["setting_order"]["system_list_ordered"].at(i).is_number_integer()){
                    system_list_ordered[i] = data["setting_order"]["system_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["video_setting_list_ordered"].is_array() && data["setting_order"]["video_setting_list_ordered"].size() == video_setting_list_ordered.size()){
            for(int32_t i = 0; i < data["setting_order"]["video_setting_list_ordered"].size(); i++){
                if(data["setting_order"]["video_setting_list_ordered"].at(i).is_number_integer()){
                    video_setting_list_ordered[i] = data["setting_order"]["video_setting_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["photo_setting_list_ordered"].is_array() && data["setting_order"]["photo_setting_list_ordered"].size() == photo_setting_list_ordered.size()){
            for(int32_t i = 0; i < data["setting_order"]["photo_setting_list_ordered"].size(); i++){
                if(data["setting_order"]["photo_setting_list_ordered"].at(i).is_number_integer()){
                    photo_setting_list_ordered[i] = data["setting_order"]["photo_setting_list_ordered"].at(i).get<int32_t>();
                }
            }
        }
        if(data["setting_order"]["video_protune_list_ordered"].is_array() && data["setting_order"]["video_protune_list_ordered"].size() == video_protune_list_ordered.size()){
            for(int32_t i = 0; i < data["setting_order"]["video_protune_list_ordered"].size(); i++){
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
