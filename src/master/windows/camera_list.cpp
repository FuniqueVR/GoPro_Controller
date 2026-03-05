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
        float width = ImGui::GetWindowSize().x;
        int32_t limit = static_cast<int32_t>(width / rect_size.x);
        int32_t counter = 0;
        int32_t size = master->getCameras().size();
        ImGui::Text("Cal: %f/%f, Total: %d, Line: %d", width, rect_size.x, size, limit);
        for(const auto& c : master->getCameras()){
            if(c){
                try{
                    if(size_event == 0) draw_line(c);
                    else {
                        draw_group(c);
                        if(counter + 1 < limit){
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
    uint32_t col_orange = IM_COL32(230, 230, 10, 255);
    uint32_t col_orange_dark = IM_COL32(80, 80, 10, 255);
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
        bool batteryCharging = false;
        int precentage = 0;
        uint32_t col_inner_color;
        if(status[std::to_string(BATTERY_PRESENT_ID)].is_number()){
            batteryHave = status[std::to_string(BATTERY_PRESENT_ID)].get<int32_t>() == 1;
        }
        if(status[std::to_string(INTERNAL_BATTERY_PERCENTAGE_ID)].is_number()){
            precentage = status[std::to_string(INTERNAL_BATTERY_PERCENTAGE_ID)].get<int32_t>();
        }
        if(status[std::to_string(INTERNAL_BATTERY_BARS_ID)].is_number()){
            batteryCharging = status[std::to_string(INTERNAL_BATTERY_BARS_ID)].get<int32_t>() == 4;
        }
        ImVec2 battery_text_size;
        std::string battery_text;
        if(batteryHave){
            if(precentage <= 25){
                col_inner_color = col_orange;
            }else{
                col_inner_color = col_greed;
            }
            battery_text = std::to_string(precentage) + "%";
        }else{
            col_inner_color = col_red;
            battery_text = "X";
        }
        battery_text_size = ImGui::CalcTextSize(battery_text.c_str());
        ImVec2 bettery_size = ImVec2(rect_size_unit.x * 2.2, rect_size_unit.y * 1.2F);
        ImVec2 frame_padding = ImVec2(10, 10);
        ImVec2 inner_padding = ImVec2(2, 2);
        int32_t spacing = 15;

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
        float w = inner_max.y - inner_min.y;
        float wc = w * (precentage / 100.0F);
        float wr = w - wc;
        inner_max.x -= wr;
        draw_list->AddRectFilled(
            inner_min, 
            inner_max, 
            col_inner_color);

        // Drawing Battery text
        ImVec2 text_pos = ImVec2(outter_max.x - battery_text_size.x, outter_max.y);
        draw_list->AddText(text_pos, col_white, battery_text.c_str());

        if(batteryCharging){
            std::string charging_text = "C=>";
            ImVec2 charging_text_size = ImGui::CalcTextSize(charging_text.c_str());
            ImVec2 charging_text_pos = ImVec2(inner_min.x, 
                ((inner_max.y + inner_min.y) / 2.0F) - (charging_text_size.y / 2.0F)
            );
            draw_list->AddText(charging_text_pos, col_orange_dark, charging_text.c_str());
        }
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
        ImVec2 frame_padding = ImVec2(10, 10);
        ImVec2 inner_padding = ImVec2(2, 2);
        int32_t spacing = 15;

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
        ImVec2 text_pos = ImVec2(outter_min.x, outter_max.y);
        draw_list->AddText(text_pos, col_white, sd_text.c_str());
    }
    // Center text
    {
        std::string record_time;
        std::string camera_title = c->name;
        std::string iso_setting;
        std::string white_balance_setting;
        float spacing = 0.75F;

        ImVec2 camera_title_size = ImGui::CalcTextSize(camera_title.c_str());

        // Draw Title in center
        ImVec2 center = ImVec2(
            image_pos.x + (rect_size.x / 2.0F),
            image_pos.y + (rect_size.y / 2.0F)
        );
        ImVec2 camera_title_min = ImVec2(
            center.x + ( camera_title_size.x / -2.0F ),
            center.y + ( camera_title_size.y / -2.0F ) + (rect_size_unit.y * (spacing * -3.0F))
        );
        draw_list->AddText(camera_title_min, col_white, camera_title.c_str());
        if(c->connected) {
            if(status[std::to_string(VIDEO_ENCODING_DURATION_ID)].is_number_integer()){
                int32_t re = status[std::to_string(VIDEO_ENCODING_DURATION_ID)].get<int32_t>();
                record_time = std::to_string(re);
            }
            if(setting[std::to_string(ISO_MIN_ID)].is_number() && setting[std::to_string(ISO_MAX_ID)].is_number()){
                int32_t iso_min = setting[std::to_string(ISO_MIN_ID)].get<int32_t>();
                int32_t iso_max = setting[std::to_string(ISO_MAX_ID)].get<int32_t>();
                std::string iso_min_text = ISO_STRING[iso_min];
                std::string iso_max_text = ISO_STRING[iso_max];
                iso_setting = "ISO " + iso_min_text + " " + iso_max_text;
            }
            if(setting[std::to_string(WHITE_BALANCE_ID)].is_number()){
                int32_t white_bal = setting[std::to_string(WHITE_BALANCE_ID)].get<int32_t>();
                white_balance_setting = WHITE_BALANCE_STRING[white_bal];
            }

            ImVec2 record_time_size = ImGui::CalcTextSize(record_time.c_str());
            ImVec2 record_time_min = ImVec2(
                center.x + ( record_time_size.x / -2.0F ),
                center.y + ( record_time_size.y / -2.0F ) + (rect_size_unit.y * (spacing * -1.0F))
            );
            draw_list->AddText(record_time_min, col_white, record_time.c_str());

            ImVec2 iso_setting_size = ImGui::CalcTextSize(iso_setting.c_str());
            ImVec2 iso_setting_min = ImVec2(
                center.x + ( iso_setting_size.x / -2.0F ),
                center.y + ( iso_setting_size.y / -2.0F ) + (rect_size_unit.y * (spacing * 1.0F))
            );
            draw_list->AddText(iso_setting_min, col_white, iso_setting.c_str());

            ImVec2 white_balance_setting_size = ImGui::CalcTextSize(white_balance_setting.c_str());
            ImVec2 white_balance_setting_min = ImVec2(
                center.x + ( white_balance_setting_size.x / -2.0F ),
                center.y + ( white_balance_setting_size.y / -2.0F ) + (rect_size_unit.y * (spacing * 3.0F))
            );
            draw_list->AddText(white_balance_setting_min, col_white, white_balance_setting.c_str());
        }
    }
    // Preset mode
    if(c->connected){
        int32_t preset;
        std::string preset_text;
        if(status[std::to_string(PRESET_ID)].is_number_integer()){
            preset = status[std::to_string(PRESET_ID)].get<int32_t>();
        }

        if(preset == 0) preset_text = "V";
        else if(preset == 65542) preset_text = "PB";
        else if(preset == 65536) preset_text = "PS";
        else if(preset == 131072) preset_text = "T";
        else if(preset == 131075) preset_text = "TT";
        else if(preset == 131076) preset_text = "TLP";
        else if(preset == 131077) preset_text = "TLT";
        else if(preset == 131073) preset_text = "TV";
        else if(preset == 131074) preset_text = "TNV";
        else preset_text = std::to_string(preset);

        ImVec2 frame_padding = ImVec2(10, 10);
        ImVec2 preset_text_size = ImGui::CalcTextSize(preset_text.c_str());
        draw_list->AddText(ImVec2(
            image_pos.x + frame_padding.x,
            (image_pos.y + rect_size.y) - (frame_padding.y + preset_text_size.y)
        ), col_white, preset_text.c_str());
    }
    // Setting
    if(c->connected){
        float spacing = 0.75F;
        float word_spacing = 10;
        std::string res;
        std::string fps;
        std::string profile;
        std::string framing;

        if(setting[std::to_string(VIDEO_RESOLUTION_ID)].is_number_integer()){
            int32_t re = setting[std::to_string(VIDEO_RESOLUTION_ID)].get<int32_t>();
            res = VIDEO_RESOLUTION_STRING[re];
        }
        if(setting[std::to_string(FRAMES_PER_SECOND_ID)].is_number_integer()){
            int32_t re = setting[std::to_string(FRAMES_PER_SECOND_ID)].get<int32_t>();
            fps = FRAMES_PER_SECOND_STRING[re];
        }
        if(setting[std::to_string(PROFILES_ID)].is_number_integer()){
            int32_t re = setting[std::to_string(PROFILES_ID)].get<int32_t>();
            profile = PROFILES_STRING[re];
        }
        if(setting[std::to_string(FRAMING_ID)].is_number_integer()){
            int32_t re = setting[std::to_string(FRAMING_ID)].get<int32_t>();
            framing = FRAMING_STRING[re];
            while(framing.size() > 1){
                framing.pop_back();
            }
        }

        ImVec2 res_text_size = ImGui::CalcTextSize(res.c_str());
        ImVec2 fps_text_size = ImGui::CalcTextSize(fps.c_str());
        ImVec2 profile_text_size = ImGui::CalcTextSize(profile.c_str());
        ImVec2 framing_text_size = ImGui::CalcTextSize(framing.c_str());
        ImVec2 frame_padding = ImVec2(5, 5);

        ImVec2 corner = image_pos + rect_size;
        ImVec2 profile_pos = ImVec2(
            corner.x - (frame_padding.x + profile_text_size.x),
            corner.y - (frame_padding.y + profile_text_size.y + ((rect_size_unit.y * 2.0F) * spacing))
        );
        draw_list->AddText(profile_pos, col_white, profile.c_str());

        ImVec2 framing_pos = ImVec2(
            corner.x - (frame_padding.x + framing_text_size.x),
            corner.y - (frame_padding.y + framing_text_size.y)
        );
        draw_list->AddText(framing_pos, col_white, framing.c_str());

        ImVec2 fps_pos = framing_pos - ImVec2(word_spacing + fps_text_size.x, 0);
        draw_list->AddText(fps_pos, col_white, fps.c_str());

        ImVec2 res_pos = fps_pos - ImVec2(word_spacing + res_text_size.x, 0);
        draw_list->AddText(res_pos, col_white, res.c_str());
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
    if(ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()){
        onClick(c);
    }
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
        ImGui::OpenPopupOnItemClick();
    }
    if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered() && c->connected){
        master->preview_start(c->server, c->ip);
        state->preview_ip = c->ip;
        state->preview_server = c->server;
        state->command_sender("preview_start");
    }

    if(ImGui::BeginPopupContextItem((title + "##Popup_Menu" + c->ip).c_str())){
        ImGui::BeginDisabled(!c->connected);
        if (ImGui::Selectable("Connect")){
            master->command_only(c->server, "usb_on", c->ip);
        }
        if (ImGui::Selectable("Disconnect")){
            master->command_only(c->server, "usb_off", c->ip);
        }
        ImGui::Separator();
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