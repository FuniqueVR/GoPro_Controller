#pragma once
#include "base_window.h"

class CameraListWindow : public BaseWindow {
public:
    CameraListWindow(json* _setting, GoProMaster* _master);
    virtual ~CameraListWindow();

    virtual void render() override;

private:

};