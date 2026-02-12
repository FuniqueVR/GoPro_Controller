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
                }catch(const std::exception& ex){
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
    }
    ImGui::End();
}