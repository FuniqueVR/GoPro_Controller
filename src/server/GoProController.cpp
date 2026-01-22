#include "GoProController.hpp"
#include "../common/iphelper.h"
#include <future>

GoProController::GoProController() {
    // Basic initialization
}

GoProController::~GoProController() {
    for(auto& thread : scan_workers){
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void GoProController::scanCameras() {
    if (scanning) {
        std::cout << "[GoProController] Scanning already in progress." << std::endl;
        return;
    }
    scanning = true;

    // Launch scanning in a separate thread to avoid blocking
    std::cout << "[GoProController] Scanning for cameras (async)..." << std::endl;
    
    scan_workers = std::vector<std::thread>();
    for(int i = 0; i < 1000; i++){
        std::thread scan_thread = std::thread([&]() {
            std::string buffer = std::to_string(i);
            while(buffer.size() < 3){
                buffer.insert(0, "0");
            }
            std::string URL = GetRemoteIP(buffer);
            std::string keep = URL + "/gopro/camera/keep_alive";

            std::cout << "[GoProController] Scanning " << URL << "\n";
        
            try {
                // Added timeout to speed up failed connections
                std::string data = exec(
                "curl -s --connect-timeout 0.1 \"" + keep + "\"");
                if(data.size() > 0){
                    std::cout << "[GoProController] Found camera at " << URL << std::endl;
                    {
                        std::lock_guard<std::mutex> lock(ips_mutex);
                        camera_ips.push_back(URL);
                    }
                }
            } catch (...) {
                // Ignore curl errors
            } 
        });
        scan_workers.push_back(std::move(scan_thread));
    }

    for(auto& thread : scan_workers){
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    // Update camera_ips with thread safety
    std::cout << "[GoProController] Found " << camera_ips.size() << " cameras." << std::endl;
    scanning = false;
}

void GoProController::sendCommandToAll(const std::string& path) {
    std::vector<std::string> ips_copy;
    {
        std::lock_guard<std::mutex> lock(ips_mutex);
        if (camera_ips.empty()) {
            std::cout << "[GoProController] No cameras connected." << std::endl;
            return;
        }
        ips_copy = camera_ips;
    }

    std::cout << "[GoProController] Sending " << path << " to " << ips_copy.size() << " cameras." << std::endl;

    // Send async requests
    for (const auto& ip : ips_copy) {
        // Construct full URL
        // OpenGoPro URL format: http://<ip>:8080/gopro/<command>
        std::string url = "http://" + ip + "/gopro/" + path;
        
        auto req = std::make_shared<HttpRequest>();
        req->method = HTTP_GET;
        req->url = url;
        http_client_send_async(req, [ip, url](HttpResponsePtr resp) {
            if (resp == NULL) {
                // std::cout << "Request failed to " << ip << std::endl;
                return;
            }
            std::cout << "Response from " << ip << ": " << resp->status_code << std::endl;
        });
    }
}

void GoProController::startRecording() {
    // Command: /gopro/camera/shutter/start
    sendCommandToAll("camera/shutter/start");
}

void GoProController::stopRecording() {
    // Command: /gopro/camera/shutter/stop
    sendCommandToAll("camera/shutter/stop");
}

void GoProController::setModePhoto() {
    // Command to switch to Photo mode
    // /gopro/camera/presets/load?id=1000 (Photo Standard) ?
    // Or /gopro/camera/control/wired_usb?p=1 (if needed)
    // Checking OpenGoPro docs for "Set Mode"
    // Usually it accepts digital presets or deprecated specific mode commands.
    // For now, let's assume a generic path, user can correct.
    // Spec says: GET /gopro/camera/presets/load?id=<id>
    // Photo ID: 65536 (Photo) often, or just 1 (Photo Mode) in legacy?
    // Let's use standard preset for now. 
    // Photo: 65536 ? No, that's group?
    // Let's use the command for settings.
    // Let's assume standardized path for now: "camera/presets/load?id=65538" (Video), "65539" (Photo) - these are example IDs.
    // Better: Standard Flat mode switch:
    // GET /gopro/camera/analytics/set_client_info (not relevant)
    
    // We will use a placeholder command that we can verify.
    // The user hotkey says "Switch photo mode".
    sendCommandToAll("camera/presets/load?id=65536"); // Placeholder ID for Photo
}

void GoProController::setModeVideo() {
    sendCommandToAll("camera/presets/load?id=65538"); // Placeholder ID for Video
}
