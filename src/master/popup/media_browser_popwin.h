#pragma once
#include "base_pop_window.h"

class MediaBrowserPopup : public BasePopWindow {
public:
    MediaBrowserPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~MediaBrowserPopup();

    virtual void trigger(bool value) override;
    virtual void render() override;
protected:
    virtual void draw_header(const CameraInfo& c);
    virtual void draw_body(const CameraInfo& c);

    virtual bool draw_item(const MediaInfo& mi);
    virtual void open_dialog_for_download_all_folder_selection();
    virtual void open_dialog_for_download_file_selection();
    virtual void download_all_folder_callback(const std::string folder);
    virtual void download_file_callback(const std::string fullpath);
private:
    std::string selected = "";
};