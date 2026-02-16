/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "camera_list.h"

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
        state->current_setting_items_bind = false;
        state->current_camera_item = c->ip;
        state->current_camera_name = c->name;
        std::cout << "Select camera: " << c->ip << std::endl;
        master->query_only(c->server, "get", c->ip);
        //current_setting_items_bind = master.getSettingsFromCamera(*c, current_setting_items);
    }
    item_event(c);
}

void CameraListWindow::draw_group(const std::shared_ptr<CameraInfo>& c){
    ImGui::BeginGroup();
    ImGuiStyle& style = ImGui::GetStyle();
    float fontsize = ImGui::GetFontSize();
    std::string id = c->ip + "##Grid_Item_ID";
    std::string pid = c->ip + "##Grid_Popup_Item_ID";
    ImGui::PushID(id.c_str());
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 image_pos = ImGui::GetCursorScreenPos();
    ImVec2 rect_size = get_rect_size();
    ImVec2 image_pos_max = image_pos + rect_size;
    uint32_t col_white = IM_COL32(255, 255, 255, 255);
    uint32_t col_grey = IM_COL32(210, 210, 210, 255);
    uint32_t col = IM_COL32(255, 0, 0, 255);
    if(c->connected) col = IM_COL32(0, 255, 0, 255);

    draw_list->AddRect(image_pos + ImVec2(5, 5), image_pos_max - ImVec2(5, 5), col, 2.0F, 0, 2.0F);

    ImVec2 title_pos_size = ImGui::CalcTextSize(c->name.c_str());
    ImVec2 title_pos = image_pos + ImVec2((rect_size.x / 2.0F) - (title_pos_size.x / 2), (rect_size.y / 2.0F) - (title_pos_size.y / 2));
    draw_list->AddText(title_pos, col_white, c->name.c_str());

    int32_t have_battery = 0;
    int32_t bar_battery = 0;
    int32_t overheating = 0;
    if(c->state["status"].is_object()){
        if(c->state["status"]["1"].is_number_integer()){
            have_battery = c->state["status"]["1"].get<int32_t>();
        }
        if(c->state["status"]["2"].is_number_integer()){
            bar_battery = c->state["status"]["2"].get<int32_t>();
        }
        if(c->state["status"]["6"].is_number_integer()){
            overheating = c->state["status"]["6"].get<int32_t>();
        }
    }

    ImVec2 cursor = ImGui::GetCursorPos();
    
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
        state->camera_selection = c->ip;
    }
    if(ImGui::IsItemClicked(ImGuiMouseButton_Right)){
        ImGui::OpenPopupOnItemClick();
    }
    if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
        master->preview_start(c->server, c->ip);
        state->command_sender("preview_start");
    }

    if(ImGui::BeginPopupContextItem((title + "##Popup_Menu").c_str())){
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
            state->command_sender("preview_start");
        }
        if (ImGui::Selectable("Delete"))
        {
            master->command_only(c->server, "delete", c->ip);
        }
        ImGui::EndPopup();
    }
}

ImVec2 CameraListWindow::get_rect_size(){
    return ImVec2(10 * (size + 5) + 20, 10 * (size + 5) + 20);
}