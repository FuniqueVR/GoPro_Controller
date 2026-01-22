#include <iostream>
#include "hv/WebSocketServer.h"
#include "hv/EventLoop.h"
#include "GoProController.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void ExecuteCommand(std::vector<const WebSocketChannelPtr&> hosts, json j){
    std::string name = j["name"].get<std::string>();
    for(auto host : hosts){
        host->send("Execute command: ");
    }
}

void QueryAction(const WebSocketChannelPtr& channel, json j){

}

int main() {
    std::cout << "Starting GoPro Server (RPi)..." << std::endl;

    std::vector<const WebSocketChannelPtr&> hosts = std::vector<const WebSocketChannelPtr&>();
    GoProController controller;
    controller.scanCameras();

    hv::WebSocketService ws;
    ws.onopen = [&](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        printf("Client connected: %s\n", channel->peeraddr().c_str());
        hosts.push_back(channel);
    };
    ws.onmessage = [&](const WebSocketChannelPtr& channel, const std::string& msg) {
        printf("Received: %s\n", msg.c_str());
        json j = json::parse(msg.c_str());
        // Simple command parsing
        if (j["key"].get<std::string>() == "command") {
            ExecuteCommand(hosts, j["value"]);
        }
        else if (j["key"].get<std::string>() == "query") {
            QueryAction(channel, j["value"]);
        }
    };
    ws.onclose = [&](const WebSocketChannelPtr& channel) {
        printf("Client disconnected\n");
        for(int i = 0; i < hosts.size(); i++){
            bool find = std::strcmp(hosts[i]->peeraddr().c_str(),
            channel->peeraddr().c_str());
            if(find){
                hosts.erase(hosts.begin() + i);
                break;
            }
        }
    };

    hv::WebSocketServer server;
    server.registerWebSocketService(&ws);
    server.setPort(9090);

    std::cout << "WebSocket Server listening on port 9090..." << std::endl;
    server.run();

    return 0;
}
