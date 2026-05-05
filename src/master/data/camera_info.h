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
/**
 * Basically holds the information of the camera
 * And which websocket server its from
 */
struct CameraInfo {
    std::string name;
    std::string last_media;
    std::string serial;
    /**
     * Camera IP
     */
    std::string ip;
    /**
     * Websocket server ip
     */
    std::string server;
    bool connected;
    /**
     * The json states
     */
    json state = json::object();
    json hw = json::object();
};