#include "inspector.h"

InspectorWindow::InspectorWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Inspector";
}

InspectorWindow::~InspectorWindow(){
    
}

void InspectorWindow::render(){

}