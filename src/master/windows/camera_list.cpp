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

void CameraListWindow::render(){
    ImGui::Begin(title.c_str(), &enable, w_flag);
    {
        std::lock_guard<std::mutex> lock(master->camera_mtx);
        for(const auto& c : master->getCameras()){
            if(c){
                try{
                    ImGui::BeginGroup();
                    ImDrawList* draw_list = ImGui::GetWindowDrawList();
                    ImVec2 image_pos = ImGui::GetCursorScreenPos();
                    ImVec2 image_pos_max = image_pos + ImVec2(10, 10);
                    draw_list->AddRectFilled(image_pos, image_pos_max, IM_COL32(255, 255, 255, 255));
                    ImGui::EndGroup();
                    /**
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
                    */
                }catch(const std::exception& ex){
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
    }
    ImGui::End();
}