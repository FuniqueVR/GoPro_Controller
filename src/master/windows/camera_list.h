#pragma once
#include "base_window.h"

class CameraListWindow : public BaseWindow {
public:
    CameraListWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    virtual ~CameraListWindow();

    virtual void render() override;

private:

};