#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "hv/requests.h"

class GoProController {
public:
    GoProController();
    ~GoProController();

    void scanCameras(); // Find connected cameras
    void startRecording();
    void stopRecording();
    void setModePhoto();
    void setModeVideo();

    // Helper to send command to all
    void sendCommandToAll(const std::string& path);

private:
    std::vector<std::thread> scan_workers;
    std::vector<std::string> camera_ips;
    std::mutex ips_mutex;
    std::atomic<bool> scanning{false}; // To track scanning state if needed
};
