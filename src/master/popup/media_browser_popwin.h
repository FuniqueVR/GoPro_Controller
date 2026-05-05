#pragma once
#include "base_pop_window.h"

class MediaBrowserPopup : public BasePopWindow {
public:
    MediaBrowserPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~MediaBrowserPopup();

    //virtual void trigger(bool value) override;
    //virtual void render() override;
private:

};