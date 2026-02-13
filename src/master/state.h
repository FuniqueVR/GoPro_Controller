/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

typedef void(*CommandSenderFunc)(const char* cmd);
typedef void(*ActionFunc)();

enum class InspectorObjectType {
    Camera
};

struct GlobalState {
    bool done;
    // Selection
    std::string websocket_server_selection;
    std::string camera_selection;
    std::string current_mode_item_string;
    int32_t current_mode_item;
    // Current select camera setting
    std::string current_camera_name = "";
    std::string current_download_location = "";
    json current_setting_items;
    bool current_setting_items_bind = false;
    json current_status_items;
    bool current_status_items_bind = false;
    // Current select camera IP address
    std::string current_camera_item = "";
    // Apply state
    std::string apply_all_item_string = "Video Resolution";
    int32_t apply_all_item = 2;
    // Caller
    CommandSenderFunc command_sender = NULL; 
    ActionFunc update_server = NULL;
    // Inspector
    InspectorObjectType iot = InspectorObjectType::Camera;
};