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
    virtual void render() override;
protected:
    virtual void draw_header(const CameraInfo& c);
    virtual void draw_body(const CameraInfo& c);

    virtual bool draw_item(const MediaInfo& mi);
private:
    int32_t selected = -1;
};