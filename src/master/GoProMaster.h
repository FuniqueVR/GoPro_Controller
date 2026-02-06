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
typedef void (*camera_log_feedback)(std::string key, std::string value);

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

    // ----------------------------------------------------------
    //
    //       Websocket Server Action
    //
    // ----------------------------------------------------------
    /**
     * Menually add websocket server and connect to it
     */
    std::string addServer(const std::string& ip);
    /**
     * Reconnect all the exists websocket server record
     */
    void reconnectAll();
    /**
     * Disconnect all the exists websocket server record
     */
    void disconnectAll();
    /**
     * Remove all the disconnected websocket server record
     */
    void cleanAll();
    /**
     * Reconnect server by ip, If not found: do nothing
     */
    void reconnect(const std::string& ip);
    /**
     * Disconnect server by ip, If not found: do nothing
     */
    void disconnect(const std::string& ip);
    /**
     * Remove server by ip, If not found: do nothing
     */
    void clean(const std::string& ip);

    // ----------------------------------------------------------
    //
    //       Websocket Server Sending Signal
    //
    // ----------------------------------------------------------
    void command_only(const std::string command, std::string target = "");
    void command_only(const std::string server, const std::string command, std::string target = "");
    void query_only(const std::string command, std::string target = "");
    void query_only(const std::string server, const std::string command, std::string target = "");
    void webcam_only(const std::string command, std::string target = "");
    void webcam_only(const std::string server, const std::string command, std::string target = "");
    void webcam_start(const std::string server);

    void presetSwitch(const std::string server, int32_t mode);
    void apply(const std::string& ip, const int32_t id, const int32_t value);
    void applyAll(const std::string& ip, const json& res);

    /**
     * Register the feedback event
     * Called when fetch inspector setting data
     */
    void registerCameraSettingFeedback(camera_setting_feedback v);
    /**
     * Register the feedback event
     * Called when fetch Monitor data
     */
    void registerCameraStatusFeedback(camera_status_feedback v);
    void registerCameraLogFeedback(camera_log_feedback v);

    /**
     * Camera list multithread lock guard
     * This will prevent race condition
     */
    std::mutex camera_mtx;

    // ----------------------------------------------------------
    //
    //       Cache Data Getter
    //
    // ----------------------------------------------------------
    /**
     * Get current camera record
     */
    const std::vector<std::shared_ptr<CameraInfo>>& getCameras() const;
    /**
     * Get current websocket server record
     */
    const std::vector<std::shared_ptr<ServerConnection>>& getServers() const;
private:
    /** 
     * All cameras record for master
    */
    std::vector<std::shared_ptr<CameraInfo>> cameras;
    /** 
     * All websocket servers record for master
    */
    std::vector<std::shared_ptr<ServerConnection>> servers;
    /**
     * Basically a update thread, THere is a while true loop in it,
     * And when done is true, it automatically escape the loop
     */
    std::thread t1;
    /**
     * Tells thread, if this websocket finish the ip query
     * This prevent command stacking, when last ip fetch is not finish yet
     */
    std::unordered_map<std::string, bool> ipQueryFinish = std::unordered_map<std::string, bool>();
    /**
     * Tells thread, if this websocket finish the state query
     * This prevent command stacking, when last state fetch is not finish yet
     */
    std::unordered_map<std::string, bool> stateQueryFinish = std::unordered_map<std::string, bool>();
    camera_setting_feedback _camera_setting_feedback = NULL;
    camera_status_feedback _camera_status_feedback = NULL;
    camera_log_feedback _camera_log_feedback = NULL;
    /**
     * Is app exit or not flag
     */
    bool done = false;

    /**
     * The background thread for fetch update from all websocket server and update etc...
     */
    void update();
    void processMessage(const std::string& ip, const std::string& msg);
    void sendToAll(const std::string& msg);
    /**
     * Clean all cameras by server ip
     */
    void cleanCameraFromServer(const std::string server);
    /**
     * Base on the new ip list
     * Appending new ip that dont appear old ips
     * Remove ip that does not exists in the new ip list
     */
    void replaceCameraFromServer(const std::string server, const std::vector<std::string> ips);

public:
    void setdone();
    /**
     * We will need to convert the Value ID to index here
     * It's easier for me to display stuff on the gui this way
     */
    bool getSettingsFromCamera(CameraInfo target, json& res);
    bool getStatusFromCamera(CameraInfo target, json&& res);
    std::string getBarInfo(const std::string camera_ip);

    int32_t findCamera(const std::string ip);
    int32_t findServer(const std::string ip);
    int32_t findCamera(const std::string server, const std::string ip);
};
