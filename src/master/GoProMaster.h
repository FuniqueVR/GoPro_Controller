/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include "hv/WebSocketClient.h"
#include <nlohmann/json.hpp>
#include "../common/iphelper.h"
#include "../common/camera_code.h"

using json = nlohmann::json;
/**
 * Basically holds the information of the camera
 * And which websocket server its from
 */
struct CameraInfo {
    /**
     * Camera IP
     */
    std::string ip;
    /**
     * Websocket server ip
     */
    std::string server;
    /**
     * The json states
     */
    json state;
};

typedef void (*camera_setting_feedback)(json setting);
typedef void (*camera_status_feedback)(json status);

/**
 * Basically holds the Websocket instance
 */
struct ServerConnection {
    /**
     * Websocket IP
     */
    std::string ip;
    /**
     * The libhv websocket client
     */
    hv::WebSocketClient client;
    /**
     * Current connection state
     */
    bool connected = false;
    /**
     * last message received from the server
     */
    std::string last_message;
};


/**
 * GoPro Master Worker
 * Use this hub stuff to control multiple websocket server or camera
 * And handles the message sender and process
 * It also use multithread to decode the message from the websocket instancess
 */
class GoProMaster {
public:
    GoProMaster();
    /**
     * Destroy the threadings and release the resource under.
     */
    ~GoProMaster();

    std::string addServer(const std::string& ip);
    void reconnectAll();
    void disconnectAll();
    void cleanAll();

    /**
     * 
     */
    void reconnect(const std::string& ip);
    void disconnect(const std::string& ip);
    void clean(const std::string& ip);

    // Commands
    void command_only(const std::string command, std::string target = "");
    void command_only(const std::string server, const std::string command, std::string target = "");
    void query_only(const std::string command, std::string target = "");
    void query_only(const std::string server, const std::string command, std::string target = "");
    void webcam_only(const std::string command, std::string target = "");
    void webcam_only(const std::string server, const std::string command, std::string target = "");
    void webcam_start(const std::string server);

    void presetSwitch(const std::string server, int32_t mode);
    bool applyAll(const std::string& ip, const json& res);

    void registerCameraSettingFeedback(camera_setting_feedback v);
    void registerCameraStatusFeedback(camera_status_feedback v);

    std::mutex camera_mtx;

    // Data for UI
    const std::vector<std::shared_ptr<CameraInfo>>& getCameras() const;
    const std::vector<std::shared_ptr<ServerConnection>>& getServers() const;
private:
    std::vector<std::shared_ptr<CameraInfo>> cameras;
    std::vector<std::shared_ptr<ServerConnection>> servers;
    std::thread t1;
    std::unordered_map<std::string, bool> ipQueryFinish = std::unordered_map<std::string, bool>();
    camera_setting_feedback _camera_setting_feedback = NULL;
    camera_status_feedback _camera_status_feedback = NULL;

    bool done = false;

    void update();
    void processMessage(const std::string& ip, const std::string& msg);
    void sendToAll(const std::string& msg);
    void cleanCameraFromServer(const std::string ip);

public:
    void setdone();
    /**
     * We will need to convert the Value ID to index here
     * It's easier for me to display stuff on the gui this way
     */
    bool getSettingsFromCamera(CameraInfo target, json& res);
    bool getStatusFromCamera(CameraInfo target, json&& res);

    int32_t findCamera(const std::string ip);
    int32_t findServer(const std::string ip);
    int32_t findCamera(const std::string server, const std::string ip);
};
