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