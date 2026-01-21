#include <iostream>
#include "hv/WebSocketServer.h"
#include "hv/EventLoop.h"
#include "GoProController.hpp"

int main() {
    std::cout << "Starting GoPro Server (RPi)..." << std::endl;

    GoProController controller;
    controller.scanCameras();

    hv::WebSocketService ws;
    ws.onopen = [](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        printf("Client connected: %s\n", channel->peeraddr().c_str());
    };
    ws.onmessage = [&controller](const WebSocketChannelPtr& channel, const std::string& msg) {
        printf("Received: %s\n", msg.c_str());
        
        // Simple command parsing
        if (msg == "f2_start_recording") {
            controller.startRecording();
            channel->send("ACK: Started Recording");
        } else if (msg == "f3_stop_recording") {
            controller.stopRecording();
            channel->send("ACK: Stopped Recording");
        } else if (msg == "f4_mode_photo") {
            controller.setModePhoto();
            channel->send("ACK: Switched to Photo Mode");
        } else if (msg == "f5_mode_video") {
            controller.setModeVideo();
            channel->send("ACK: Switched to Video Mode");
        } else {
            channel->send("ERR: Unknown Command");
        }
    };
    ws.onclose = [](const WebSocketChannelPtr& channel) {
        printf("Client disconnected\n");
    };

    hv::WebSocketServer server;
    server.registerHttpService(&ws);
    server.setPort(9090);

    std::cout << "WebSocket Server listening on port 9090..." << std::endl;
    server.run();

    return 0;
}
