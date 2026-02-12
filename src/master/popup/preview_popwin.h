#pragma once
#include "base_pop_window.h"

class PreviewPopup : public BasePopWindow {
public:
    PreviewPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~PreviewPopup();

    virtual void render() override;
private:
    
};
