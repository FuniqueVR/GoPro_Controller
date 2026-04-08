/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once

#include <utility>
#include <vector>
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

class GoProController {
public:
    GoProController();
    ~GoProController();
    // Camera part of calls
    void scanCameras();
    void cleanCameras();
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
    std::string getAllIP();
    std::string getAllModel();
    // Status part of calls
    std::string queryStatus(std::string target);
    std::string setSetting(std::string target, int32_t ID, std::string value);
    std::string setSettingAll(std::string target, json value);
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
    // Status part of calls
    std::vector<std::pair<std::string, std::string>> _queryAllStatus(std::vector<std::string> targets);
    std::pair<std::string, std::string> _queryStatus(std::string target);
    std::vector<std::pair<std::string, std::string>> _queryAllHW(std::vector<std::string> targets);
    std::pair<std::string, std::string> _queryHW(std::string target);
    std::vector<std::pair<std::string, std::string>> _setAllSetting(std::vector<std::string> targets, int32_t ID, std::string value);
    std::pair<std::string, std::string> _setSetting(std::string target, int32_t ID, std::string value);
    // Webcam part of calls
    void _webcamAllMode(std::vector<std::string> targets);
    void _webcamMode(std::string target);
    void _webcamAllUnMode(std::vector<std::string> targets);
    void _webcamUnMode(std::string target);
    void _webcamAllOn(std::vector<std::string> targets, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void _webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void _webcamAllOff(std::vector<std::string> targets);
    void _webcamOff(std::string target);
    std::pair<std::string, std::string> _webcamStatus(std::string target);
    std::pair<std::string, std::string> _webcamVersion(std::string target);
    // Preview part of calls
    void _previewAllOn(std::vector<std::string> targets, int32_t port);
    void _previewOn(std::string target, int32_t port);
    void _previewAllOff(std::vector<std::string> targets);
    void _previewOff(std::string target);
    // Media part of calls
    std::vector<std::pair<std::string, std::string>> _getAllMediaList(std::vector<std::string> targets);
    std::pair<std::string, std::string> _getMediaList(std::string target);
    std::vector<std::pair<std::string, std::string>> _getAllLastMedia(std::vector<std::string> targets);
    std::pair<std::string, std::string> _getLastMedia(std::string target);
    // Utility calls
    std::pair<std::string, std::string> _getSingleResponse(std::string target, std::string suffix);
    std::vector<std::pair<std::string, std::string>> _getAllResponse(std::vector<std::string> targets, std::string suffix);
private:
    mdns_cpp::mDNS mdns;
    bool mdns_scaned = false;
    std::vector<std::thread> scan_workers;
    std::vector<std::string> camera_ips;
    std::unordered_map<std::string, std::string> camera_name;
    std::mutex ips_mutex;
    std::atomic<bool> scanning{false}; // To track scanning state if needed
};
