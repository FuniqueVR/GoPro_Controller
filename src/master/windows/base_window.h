/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <chrono>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "../GoProMaster.h"
#include "../state.h"

using json = nlohmann::json;

#define w_flag ImGuiWindowFlags_NoCollapse
#define wp_flag ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize

class BaseWindow {
public:
    BaseWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    ~BaseWindow();

    bool enable;

    virtual void update();
    virtual void render();
    virtual bool is_close();
    virtual void save_setting();
protected:
    std::shared_ptr<json> setting;
    std::shared_ptr<GlobalState> state;
    std::shared_ptr<GoProMaster> master;
    std::string title = "";
};