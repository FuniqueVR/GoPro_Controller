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

void BasePopWindow::update(){

}

void BasePopWindow::render(){

}

bool BasePopWindow::is_close(){
    return enable;
}

void BasePopWindow::save_setting(){

}