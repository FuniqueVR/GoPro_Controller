#include <iostream>
#include <vector>
#include "hv/WebSocketServer.h"
#include "hv/EventLoop.h"
#include "GoProController.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<const WebSocketChannelPtr*> hosts = std::vector<const WebSocketChannelPtr*>();
GoProController controller;

void ExecuteCommand(const WebSocketChannelPtr& channel, json j){
    std::string name = j["name"].get<std::string>();
    std::string target = j["target"].get<std::string>();

    if(name == "reboot"){
        controller.reboot(target);
    }else if(name == "shutdown"){
        controller.shutdown(target);
    }else if(name == "keep_alive"){
        controller.keep_alive(target);
    }else if(name == "usb_on"){
        controller.usb_on(target);
    }else if(name == "usb_off"){
        controller.usb_off(target);
    }else if(name == "datetime"){
        controller.datetime(target);
    }else if(name == "zoom"){
        controller.zoom(target);
    }else if(name == "shutter"){
        controller.shutter(target);
    }
    else if(name == "ip"){
        std::string r = controller.getAllIP();
        channel->send(r);
    }

    for(auto host : hosts){
        host->get()->send(std::string("Execute command: ") + name);
    }
}

void QueryAction(const WebSocketChannelPtr& channel, json j){
    std::string mode = j["mode"].get<std::string>();
    std::string target = j["target"].get<std::string>();

    if(mode == "all"){

    }else if (mode == "single"){
        
    }
}

int main() {
    std::cout << "Starting GoPro Server (RPi)..." << std::endl;

    controller.scanCameras();

    hv::WebSocketService ws;
    ws.onopen = [&](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        printf("Client connected: %s\n", channel->peeraddr().c_str());
        hosts.push_back(&channel);
    };
    ws.onmessage = [&](const WebSocketChannelPtr& channel, const std::string& msg) {
        printf("Received: %s\n", msg.c_str());
        json j = json::parse(msg.c_str());
        // Simple command parsing
        if (j["key"].get<std::string>() == "command") {
            ExecuteCommand(channel, j["value"]);
        }
        else if (j["key"].get<std::string>() == "query") {
            QueryAction(channel, j["value"]);
        }
    };
    ws.onclose = [&](const WebSocketChannelPtr& channel) {
        printf("Client disconnected\n");
        for(int i = 0; i < hosts.size(); i++){
            bool find = std::strcmp(hosts[i]->get()->peeraddr().c_str(),
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
