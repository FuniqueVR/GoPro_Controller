/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "camera_list.h"

std::string bytesToGbString(long bytes) {
    // Define the value of one gigabyte (1024 * 1024 * 1024 bytes)
    const long GIGABYTE = 1024L * 1024L * 1024L; 

    // Convert bytes to gigabytes as a double
    double gigabytes = static_cast<double>(bytes) / GIGABYTE;

    // Use std::stringstream to format the output with desired precision
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << gigabytes << " GB";

    // Return the formatted string
    return ss.str();
}

CameraListWindow::CameraListWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Camera Dashboard";
}

CameraListWindow::~CameraListWindow(){
    
}

json CameraListWindow::get_window_data() {
    json d = json::object();
    d["size"] = size;
    return d;
}

void CameraListWindow::set_window_data(json data){
    if(data["size"].is_number_integer()){
        size_event = data["size"].get<int32_t>();
    }
}

void CameraListWindow::render(){
    ImGui::Begin(title.c_str(), &enable, w_flag);
    {
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        ImGui::SliderInt("Item Size##Camera_List_Size", &size_event, 0, 10);
        ImVec2 rect_size = get_rect_size();
        float width = ImGui::GetWindowWidth();
        int32_t limit = static_cast<int32_t>(width / rect_size.x);
        int32_t counter = 0;
        for(const auto& c : master->getCameras()){
            if(c){
                try{
                    if(size == 0) draw_line(c);
                    else {
                        draw_group(c);
                        if(counter < limit){
                            ImGui::SameLine();
                            counter++;
                        }else{
                            counter = 0;
                        }
                    }
                }catch(const std::exception& ex){
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
    }
    ImGui::End();
    if(size != size_event){
        size = size_event;
        state->update_server();
    }
}

void CameraListWindow::draw_line(const std::shared_ptr<CameraInfo>& c){
    bool selected = c->ip == state->current_camera_item;
    std::string plusStatus = master->getBarInfo(c);
    std::string plusID = plusStatus + "##CameraList_" + c->ip;
    if(ImGui::Selectable(plusID.c_str(), selected)){
        // User select interaction
        onClick(c);
    }
    item_event(c);
}

void CameraListWindow::draw_group(const std::shared_ptr<CameraInfo>& c){
    json status = json::object();
    json setting = json::object();
    master->getStatusFromCamera(*c, status);
    master->getSettingsFromCamera(*c, setting);

    ImGui::BeginGroup();
    ImGuiStyle& style = ImGui::GetStyle();
    float fontsize = ImGui::GetFontSize();
    std::string id = c->ip + "##Grid_Item_ID";
    std::string pid = c->ip + "##Grid_Popup_Item_ID";
    ImGui::PushID(id.c_str());

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 image_pos = ImGui::GetCursorScreenPos();
    ImVec2 rect_size = get_rect_size();
    ImVec2 rect_size_unit = ImVec2(rect_size.x / 12.0F, rect_size.y / 12.0F);

    ImVec2 image_pos_max = image_pos + rect_size;
    uint32_t col_white = IM_COL32(255, 255, 255, 255);
    uint32_t col_grey = IM_COL32(210, 210, 210, 255);
    uint32_t col_red = IM_COL32(230, 10, 10, 255);
    uint32_t col_greed = IM_COL32(10, 230, 10, 255);

    // Drawing outline
    {
        uint32_t col = col_red;
        if(c->connected) col = col_greed;
        draw_list->AddRect(image_pos + ImVec2(5, 5), image_pos_max - ImVec2(5, 5), col, 2.0F, 0, 2.0F);
    }
    // Drawing Battery
    if(c->connected){
        bool batteryHave = false;
        int precentage = 0;
        uint32_t col_inner_color;
        if(status[std::to_string(BATTERY_PRESENT_ID)].is_number()){
            batteryHave = status[std::to_string(BATTERY_PRESENT_ID)].get<int32_t>() == 1;
        }
        if(status[std::to_string(INTERNAL_BATTERY_PERCENTAGE_ID)].is_number()){
            precentage = status[std::to_string(INTERNAL_BATTERY_PERCENTAGE_ID)].get<int32_t>();
        }
        ImVec2 battery_text_size;
        std::string battery_text;
        if(batteryHave){
            col_inner_color = col_greed;
            battery_text = std::to_string(precentage) + "%";
        }else{
            col_inner_color = col_red;
            battery_text = "X";
        }
        battery_text_size = ImGui::CalcTextSize(battery_text.c_str());
        ImVec2 bettery_size = ImVec2(rect_size_unit.x * 2.2, rect_size_unit.y * 1.2F);
        ImVec2 frame_padding = ImVec2(5, 5);
        ImVec2 inner_padding = ImVec2(2, 2);

        // Drawing Battery outline
        ImVec2 outter_min = image_pos + ImVec2(rect_size.x - (frame_padding.x + bettery_size.x), frame_padding.y);
        ImVec2 outter_max = image_pos + ImVec2(rect_size.x - frame_padding.x, frame_padding.y + bettery_size.y);
        draw_list->AddRectFilled(
            outter_min, 
            outter_max, 
            col_grey);
            
        // Drawing Battery inline
        ImVec2 inner_min = outter_min + inner_padding;
        ImVec2 inner_max = outter_max - inner_padding;
        draw_list->AddRectFilled(
            inner_min, 
            inner_max, 
            col_inner_color);

        // Drawing Battery text
        ImVec2 text_pos = ImVec2(outter_min.x - battery_text_size.x, outter_min.y);
        draw_list->AddText(text_pos, col_white, battery_text.c_str());
    }
    // Drawing SD card
    if(c->connected){
        bool sdHave = false;
        int64_t remaining = 0;
        uint32_t col_inner_color;
        if(status[std::to_string(PRIMARY_STORAGE_ID)].is_number()){
            int b = status[std::to_string(PRIMARY_STORAGE_ID)].get<int32_t>() == 1;
            if(b == 1) sdHave = true;
        }
        if(status[std::to_string(SD_CARD_REMAINING_ID)].is_number()){
            remaining = status[std::to_string(SD_CARD_REMAINING_ID)].get<int64_t>();
        }
        ImVec2 sd_text_size;
        std::string sd_text;
        if(sdHave){
            col_inner_color = col_greed;
            sd_text = bytesToGbString(remaining);
        }else{
            col_inner_color = col_red;
            sd_text = "X";
        }
        sd_text_size = ImGui::CalcTextSize(sd_text.c_str());
        ImVec2 sd_size = ImVec2(rect_size_unit.x * 2.2, rect_size_unit.y * 1.2F);
        ImVec2 frame_padding = ImVec2(5, 5);
        ImVec2 inner_padding = ImVec2(2, 2);

        // Drawing SD outline
        ImVec2 outter_min = image_pos + frame_padding;
        ImVec2 outter_max = image_pos + frame_padding + sd_size;
        draw_list->AddRectFilled(
            outter_min, 
            outter_max, 
            col_grey);
            
        // Drawing SD inline
        ImVec2 inner_min = outter_min + inner_padding;
        ImVec2 inner_max = outter_max - inner_padding;
        draw_list->AddRectFilled(
            inner_min, 
            inner_max, 
            col_inner_color);

        // Drawing SD text
        ImVec2 text_pos = ImVec2(outter_max.x, outter_min.y);
        draw_list->AddText(text_pos, col_white, sd_text.c_str());
    }
    // Center text
    {
        
    }
    // Preset mode
    {
        
    }
    // Setting
    {
        
    }
    
    ImGui::PopID();
    ImGui::Dummy(rect_size);
    if(ImGui::IsItemHovered()){
        ImGui::SetItemTooltip("%s", "Test\nTest");
        draw_list->AddRect(image_pos, image_pos_max, col_grey, 2.0F, 0, 5.0F);
    }
    item_event( c);
    ImGui::EndGroup();
}

void CameraListWindow::item_event(const std::shared_ptr<CameraInfo>& c){
    if(ImGui::IsItemClicked(ImGuiMouseButton_Left)){
        onClick(c);
    }
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)){
        ImGui::OpenPopupOnItemClick();
    }
    if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
        master->preview_start(c->server, c->ip);
        state->preview_ip = c->ip;
        state->preview_server = c->server;
        state->command_sender("preview_start");
    }

    if(ImGui::BeginPopupContextItem((title + "##Popup_Menu").c_str())){
        ImGui::BeginDisabled(!c->connected);
        if (ImGui::Selectable("Reboot"))
        {
            master->command_only(c->server, "reboot", c->ip);
        }
        if (ImGui::Selectable("Shutdown"))
        {
            master->command_only(c->server, "shutdown", c->ip);
        }
        if (ImGui::Selectable("Preview"))
        {
            master->preview_start(c->server, c->ip);
            state->preview_ip = c->ip;
            state->preview_server = c->server;
            state->command_sender("preview_start");
        }
        ImGui::EndDisabled();
        if (ImGui::Selectable("Delete"))
        {
            master->command_only(c->server, "delete", c->ip);
        }
        ImGui::EndPopup();
    }
}

void CameraListWindow::onClick(const std::shared_ptr<CameraInfo>& c){
    state->current_setting_items_bind = false;
    state->current_camera_item = c->ip;
    state->current_camera_name = c->name;
    std::cout << "Select camera: " << c->ip << std::endl;
    master->query_only(c->server, "get", c->ip);
    //current_setting_items_bind = master.getSettingsFromCamera(*c, current_setting_items);
}

ImVec2 CameraListWindow::get_rect_size(){
    return ImVec2(10 * (size + 10) + 20, 10 * (size + 10) + 20);
}