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

    void scanCameras();
    void addCameras(std::string serial);
    void startRecording();
    void stopRecording();
    void setModePhoto();
    void setModeVideo();

    void reboot(std::string target);
    void shutdown(std::string target);
    void keep_alive(std::string target);
    void usb(std::string target, bool ison);
    void datetime(std::string target);
    void zoom(std::string target);
    void shutter(std::string target, bool isstart);

    std::string queryStatus(std::string target);
    std::string setSetting(std::string target, int ID, std::string value);

    void webcamMode(std::string target);
    void webcamUnMode(std::string target);
    void webcamOn(std::string target, int startPort, int res, int fov, bool TS);
    void webcamOff(std::string target);
    std::string webcamStatus(std::string target);
    std::string webcamVersion(std::string target);

    std::string getMediaList(std::string target);

    std::string getAllIP();

protected:
    void _loadRecord();
    void _updateRecord();

    void _reboot(std::string target);
    void _shutdown(std::string target);
    void _keep_alive(std::string target);
    void _usb(std::string target, bool ison);
    void _datetime(std::string target);
    void _zoom(std::string target);
    void _shutter(std::string target, bool isstart);

    std::pair<std::string, std::string> _queryStatus(std::string target);
    std::pair<std::string, std::string> _setSetting(std::string target, int ID, std::string value);

    void _webcamMode(std::string target);
    void _webcamUnMode(std::string target);
    void _webcamOn(std::string target, int startPort, int res, int fov, bool TS);
    void _webcamOff(std::string target);
    std::pair<std::string, std::string> _webcamStatus(std::string target);
    std::pair<std::string, std::string> _webcamVersion(std::string target);

    std::pair<std::string, std::string> _getMediaList(std::string target);

private:
    mdns_cpp::mDNS mdns;
    bool mdns_scaned = false;
    std::vector<std::thread> scan_workers;
    std::vector<std::string> camera_ips;
    std::mutex ips_mutex;
    std::atomic<bool> scanning{false}; // To track scanning state if needed
};
