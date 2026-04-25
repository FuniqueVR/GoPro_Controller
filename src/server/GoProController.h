/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once

#include <utility>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "mdns_cpp/logger.hpp"
#include "mdns_cpp/mdns.hpp"
#include "hv/requests.h"
#include <nlohmann/json.hpp>
#include "../common/iphelper.h"
#include "../common/camera_code.h"

/**
 * Most of the function will have first string target as parameter.
 * User can leave this field with empty string to repersent the broadcasting type,
 * This mean this type of action will send to all the connected clients.
*/

using json = nlohmann::json;
std::string getPacket(std::string key, json data);

typedef std::pair<std::string,std::string> SingleResponse;

/**
 * The cpp files break into two folder, and base on implementation detail to seperate cpp files
 * * controller (public methods)
 * * private (private methods)
 */
class GoProController {
public:
    GoProController();
    ~GoProController();
    void update();
    // Camera part of calls
    /**
     * Using mdns feature to catch all gopro services and store the ip addresses
     */
    void scanCameras();
    /**
     * Clear the array of IP address
     */
    void cleanCameras();
    /**
     * Rename the camera by ip input
     */
    void renameCameras(std::string ip, std::string name);
    void addCameras(std::string serial);
    void deleteCameras(std::string ip);
    // Control part of calls
    void setPreset(std::string target, int32_t mode);
    void reboot(std::string target);
    void shutdown(std::string target);
    void keep_alive(std::string target);
    void usb(std::string target, bool ison);
    void datetime(std::string target);
    void zoom(std::string target, int32_t value);
    void shutter(std::string target, bool isstart);
    void locate(std::string target, bool ison);
    std::string getAllIP();
    std::string getAllModel();
    // Status part of calls
    std::string queryStatus(std::string target);
    std::string setSetting(std::string target, int32_t ID, std::string value);
    /**
     * If source is null, it does not matter anyway, it just for ignore apply reason.
     * If target is null, it will the apply range to all of the clients.
     */
    std::string setSettingAll(const std::string source, const std::string target, int32_t preset, json value);
    // Webcam part of calls
    void webcamMode(std::string target);
    void webcamUnMode(std::string target);
    void webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void webcamOff(std::string target);
    std::string webcamStatus(std::string target);
    std::string webcamVersion(std::string target);
    // Preview part of calls
    void previewOn(std::string target, int32_t port);
    void previewOff(std::string target);
    // Media part of calls
    std::string getMediaList(std::string target);
    std::string getLastMedia(std::string target);
    std::string getFetchURL(std::string target_ip, bool is_local);
protected:
    // Config
    void _loadRecord();
    void _updateRecord();
    // Control part of calls
    void _setAllPreset(std::vector<std::string> targets, int32_t mode);
    void _setPreset(std::string target, int32_t mode);
    void _rebootAll(std::vector<std::string> targets);
    void _reboot(std::string target);
    void _shutdownAll(std::vector<std::string> targets);
    void _shutdown(std::string target);
    void _keepAliveAll(std::vector<std::string> targets);
    void _keepAlive(std::string target);
    void _usbAll(std::vector<std::string> targets, bool ison);
    void _usb(std::string target, bool ison);
    void _datetimeAll(std::vector<std::string> targets);
    void _datetime(std::string target);
    void _zoomAll(std::vector<std::string> targets, int32_t value);
    void _zoom(std::string target, int32_t value);
    void _shutterAll(std::vector<std::string> targets, bool isstart);
    void _shutter(std::string target, bool isstart);
    void _locate(std::string target, bool ison);
    // Status part of calls
    std::vector<SingleResponse> _queryAllStatus(std::vector<std::string> targets);
    SingleResponse _queryStatus(std::string target);
    std::vector<SingleResponse> _queryAllHW(std::vector<std::string> targets);
    SingleResponse _queryHW(std::string target);
    std::vector<SingleResponse> _setAllSetting(std::vector<std::string> targets, int32_t ID, std::string value);
    SingleResponse _setSetting(std::string target, int32_t ID, std::string value);
    std::vector<SingleResponse> _setAllSetting(std::vector<std::string> targets, int32_t preset, json res);
    std::vector<SingleResponse> _setSetting(std::string target, int32_t preset, json res);
    std::vector<SingleResponse> _setSetting_utility(std::string target, json res, std::vector<int32_t> setting_ids);
    // Webcam part of calls
    void _webcamAllMode(std::vector<std::string> targets);
    void _webcamMode(std::string target);
    void _webcamAllUnMode(std::vector<std::string> targets);
    void _webcamUnMode(std::string target);
    void _webcamAllOn(std::vector<std::string> targets, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void _webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void _webcamAllOff(std::vector<std::string> targets);
    void _webcamOff(std::string target);
    SingleResponse _webcamStatus(std::string target);
    SingleResponse _webcamVersion(std::string target);
    // Preview part of calls
    void _previewAllOn(std::vector<std::string> targets, int32_t port);
    void _previewOn(std::string target, int32_t port);
    void _previewAllOff(std::vector<std::string> targets);
    void _previewOff(std::string target);
    // Media part of calls
    std::vector<SingleResponse> _getAllMediaList(std::vector<std::string> targets);
    SingleResponse _getMediaList(std::string target);
    std::vector<SingleResponse> _getAllLastMedia(std::vector<std::string> targets);
    SingleResponse _getLastMedia(std::string target);
    // Utility calls
    SingleResponse _getSingleResponse(std::string target, std::string suffix);
    std::vector<SingleResponse> _getAllResponse(std::vector<std::string> targets, std::string suffix);

    int32_t _get_current_model(json hwinfo);
private:
    mdns_cpp::mDNS mdns;
    bool mdns_scaned = false;
    std::vector<std::thread> scan_workers;
    /**
     * @brief IP address record
     */
    std::vector<std::string> camera_ips;
    /**
     * @brief IP address record (Alive)
     */
    std::vector<std::string> camera_alive_ips;
    /**
     * @brief IP address record (Thread Guard)
     */
    std::mutex ips_mutex;
    /**
     * @brief IP address record (Alive) (Thread Guard)
     */
    std::mutex ips_alive_mutex;
    /**
     * @brief IP : Hardware info json data
     */
    std::unordered_map<std::string, json> camera_hw;
    /**
     * @brief IP : Name
     */
    std::unordered_map<std::string, std::string> camera_name;
    std::atomic<bool> scanning{false}; // To track scanning state if needed
};
