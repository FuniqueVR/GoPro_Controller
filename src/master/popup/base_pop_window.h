#pragma once
#include <chrono>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../GoProMaster.h"
#include "../data/state.h"

using json = nlohmann::json;

#define wp_flag ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize
#ifdef _WIN32
    int32_t wp_cond = ImGuiCond_Once;
#else
    int32_t wp_cond = ImGuiCond_Always;
#endif

class BasePopWindow {
public:
    BasePopWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~BasePopWindow();

    virtual json get_window_data();
    virtual void set_window_data(json data);

    /**
     * Does anyone call open popup right at this frame
     */
    std::string get_title();
    bool is_enable();
    bool is_open();
    /**
     * Is enable being triiger right now
     */
    virtual void trigger(bool value);
    virtual void update();
    /**
     * Render the imgui window
     */
    virtual void render();
    /**
     * 
     */
    virtual void detect();
protected:
    bool enable = false;
    bool isopen = false;
    std::shared_ptr<json> setting;
    std::shared_ptr<GlobalState> state;
    std::shared_ptr<GoProMaster> master;
    std::string title = "";
};