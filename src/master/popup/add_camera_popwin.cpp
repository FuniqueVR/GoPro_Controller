#include "add_camera_popwin.h"

AddCameraPopup::AddCameraPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Add Camera##Popup";
}

AddCameraPopup::~AddCameraPopup(){
    
}