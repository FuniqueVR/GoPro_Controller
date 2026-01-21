#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "hv/requests.h"

// Mock IPs for testing if no real cameras found
static const std::vector<std::string> MOCK_IPS = {
    "127.0.0.1:8081", "127.0.0.1:8082" 
};

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
    std::vector<std::string> camera_ips;
};
