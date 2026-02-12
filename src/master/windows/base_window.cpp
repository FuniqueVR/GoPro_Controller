#include "base_window.h"

BaseWindow::BaseWindow(json* _setting, std::shared_ptr<GoProMaster> _master){
    setting = _setting;
    master = _master;
}

BaseWindow::~BaseWindow(){

}

void BaseWindow::update(){

}

void BaseWindow::render(){

}

bool BaseWindow::is_close(){

}