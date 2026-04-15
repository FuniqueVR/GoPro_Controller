#pragma once
#include "base_pop_window.h"

class StudioPopup : public BasePopWindow {
public:
    StudioPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~StudioPopup();

    virtual void trigger(bool value);
    virtual void render() override;

protected:
    
};
