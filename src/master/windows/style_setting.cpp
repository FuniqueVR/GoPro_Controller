#include "style_setting.h"

StyleSetting::StyleSetting(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Style Setting";
}

StyleSetting::~StyleSetting(){
    
}

json StyleSetting::get_window_data() {
    return json::object();
}

void StyleSetting::set_window_data(json data) {

}

void StyleSetting::render(){
    ImGui::Begin("Style Setting", &enable, w_flag);
    {
    }
    ImGui::End();
}