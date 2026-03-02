#pragma once
#include "base_pop_window.h"

class ScanCameraPopup : public BasePopWindow {
public:
    ScanCameraPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~ScanCameraPopup();

    virtual void render() override;
private:
    std::string server_ip_buf = "127.0.0.1";
    std::string error = "";
};
