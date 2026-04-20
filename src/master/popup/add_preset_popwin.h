#pragma once
#include "base_pop_window.h"

class AddPresetPopup : public BasePopWindow {
public:
    AddPresetPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~AddPresetPopup();

    virtual void trigger(bool value) override;
    virtual void render() override;
private:
};