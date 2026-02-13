#pragma once
#include <chrono>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../GoProMaster.h"
#include "../state.h"

using json = nlohmann::json;

#define w_flag ImGuiWindowFlags_NoCollapse
#define wp_flag ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize

class BasePopWindow {
public:
    BasePopWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~BasePopWindow();

    bool enable = false;

    virtual void render();
    virtual void detect();
protected:
    std::shared_ptr<json> setting;
    std::shared_ptr<GlobalState> state;
    std::shared_ptr<GoProMaster> master;
    std::string title = "";
};