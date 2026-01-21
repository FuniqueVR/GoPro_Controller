#include <iostream>
#include "hv/WebSocketServer.h"
#include "hv/EventLoop.h"
#include "GoProController.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
        json j = json::parse(msg.c_str());
        // Simple command parsing
        if (j["key"].get<std::string>() == "command") {
            channel->send("Execute command: " + j["value"]["name"].get<std::string>());
        }
    };
    ws.onclose = [](const WebSocketChannelPtr& channel) {
        printf("Client disconnected\n");
    };

    hv::WebSocketServer server;
    server.registerWebSocketService(&ws);
    server.setPort(9090);

    std::cout << "WebSocket Server listening on port 9090..." << std::endl;
    server.run();

    return 0;
}
