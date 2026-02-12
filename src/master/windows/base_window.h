#pragma once
#include <chrono>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include "imgui.h"
#include "../GoProMaster.h"

using json = nlohmann::json;

#define w_flag ImGuiWindowFlags_NoCollapse
#define wp_flag ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize

class BaseWindow {
public:
    BaseWindow(json* _setting, std::shared_ptr<GoProMaster> _master);
    ~BaseWindow();

    bool enable;

    virtual void update();
    virtual void render();
    virtual bool is_close();
protected:
    json* setting;
    std::shared_ptr<GoProMaster> master;
    std::string title = "";
};