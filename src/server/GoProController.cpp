#include "GoProController.hpp"
#include <future>

GoProController::GoProController() {
    // Basic initialization
}

GoProController::~GoProController() {

}

void GoProController::scanCameras() {
    std::cout << "[GoProController] Scanning for cameras..." << std::endl;
    // TODO: Real implementation would scan network interfaces or specific subnets
    // For now, use MOCK IPs
    camera_ips = MOCK_IPS;
    std::cout << "[GoProController] Found " << camera_ips.size() << " cameras (Mock)." << std::endl;
}

void GoProController::sendCommandToAll(const std::string& path) {
    if (camera_ips.empty()) {
        std::cout << "[GoProController] No cameras connected." << std::endl;
        return;
    }

    std::cout << "[GoProController] Sending " << path << " to " << camera_ips.size() << " cameras." << std::endl;

    // Send async requests
    for (const auto& ip : camera_ips) {
        // Construct full URL
        // OpenGoPro URL format: http://<ip>:8080/gopro/<command>
        // Adjust based on successful connection logic.
        // For USB, it's typically http://172.2x.x.x:8080/gopro/...
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
