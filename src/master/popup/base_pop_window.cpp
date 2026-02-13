/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "base_pop_window.h"

BasePopWindow::BasePopWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
){
    setting = _setting;
    state = _state;
    master = _master;
}

BasePopWindow::~BasePopWindow(){

}

std::string BasePopWindow::get_title(){
    return title;
}

void BasePopWindow::trigger(bool value){

}

void BasePopWindow::render(){

}

void BasePopWindow::detect(){
    if(enable){
        enable = false;
        ImGui::OpenPopup(title.c_str());
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }
}