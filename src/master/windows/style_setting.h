#pragma once
#include "base_window.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class StyleSetting : public BaseWindow {
public:
    StyleSetting(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~StyleSetting();

    json get_window_data() override;
    void set_window_data(json data) override;
    virtual void render() override;
private:
    json buffer = json::object();
};