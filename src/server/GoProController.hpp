#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "mdns_cpp/logger.hpp"
#include "mdns_cpp/mdns.hpp"
#include "hv/requests.h"

/**
 * Most of the function will have first string target as parameter.
 * User can leave this field with empty string to repersent the broadcasting type,
 * This mean this type of action will send to all the connected clients.
*/

class GoProController {
public:
    GoProController();
    ~GoProController();

    void scanCameras(); // Find connected cameras
    void startRecording();
    void stopRecording();
    void setModePhoto();
    void setModeVideo();

    void reboot(std::string target);
    void shutdown(std::string target);
    void keep_alive(std::string target);
    void usb_on(std::string target);
    void usb_off(std::string target);
    void datetime(std::string target);
    void zoom(std::string target);
    void shutter(std::string target);

    std::string getAllIP();

protected:
    void _reboot(std::string target);
    void _shutdown(std::string target);
    void _keep_alive(std::string target);
    void _usb_on(std::string target);
    void _usb_off(std::string target);
    void _datetime(std::string target);
    void _zoom(std::string target);
    void _shutter(std::string target);

private:
    bool init = true;
    mdns_cpp::mDNS mdns;
    std::vector<std::thread> scan_workers;
    std::vector<std::string> camera_ips;
    std::mutex ips_mutex;
    std::atomic<bool> scanning{false}; // To track scanning state if needed
};
