/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "base_window.h"

BaseWindow::BaseWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
){
    setting = _setting;
    state = _state;
    master = _master;
}

BaseWindow::~BaseWindow(){

}

json BaseWindow::get_window_data() {
    return json::object();
}

void BaseWindow::set_window_data(json data){

}

std::string BaseWindow::get_title(){
    return title;
}

void BaseWindow::trigger(bool value){

}

void BaseWindow::update(){

}

void BaseWindow::render(){

}

bool BaseWindow::is_close(){
    return !enable;
}

void BaseWindow::save_setting(){

}