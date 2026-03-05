#pragma once
#include "base_pop_window.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class StyleSettingPopup : public BasePopWindow {
public:
    StyleSettingPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~StyleSettingPopup();

    virtual void render() override;
private:
    json buffer;
};