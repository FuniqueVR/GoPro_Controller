#include "preview_popwin.h"

PreviewPopup::PreviewPopup(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BasePopWindow(_setting, _state, _master) {
    title = "Preview##Popup";
}

PreviewPopup::~PreviewPopup(){
    
}

void PreviewPopup::render(){
    
}