#pragma once
#include "base_pop_window.h"

class PresetManagerPopup : public BasePopWindow {
public:
    PresetManagerPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~PresetManagerPopup();

    virtual void trigger(bool value) override;
    virtual void render() override;

    void draw_preset_list();
    void draw_preset_detail();
private:
    std::string preset_select;
    bool applying = false;
};