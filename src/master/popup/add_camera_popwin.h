#pragma once
#include "base_pop_window.h"

class AddCameraPopup : public BasePopWindow {
public:
    AddCameraPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~AddCameraPopup();

    virtual void render() override;
    virtual void detect() override;
private:
};