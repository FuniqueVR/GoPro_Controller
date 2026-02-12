#pragma once
#include "base_pop_window.h"

class StartWebcamPopup : public BasePopWindow {
public:
    StartWebcamPopup(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~StartWebcamPopup();

    virtual void render() override;
private:
    std::string server_ip_buf = "127.0.0.1";
    std::string port_buf = "7000";
    int32_t res_buf = 0;
    std::string res_string_buf = "480p";
    int32_t fov_buf = 0;
    std::string fov_string_buf = "Wide";
    bool ts_buf = true;
    std::string error = "";
};
