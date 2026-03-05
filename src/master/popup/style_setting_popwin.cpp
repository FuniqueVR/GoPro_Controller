#include "style_setting_popwin.h"

StyleSettingPopup::StyleSettingPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Start Webcam##Popup";
}

StyleSettingPopup::~StyleSettingPopup(){
    
}

void StyleSettingPopup::render(){
    
}