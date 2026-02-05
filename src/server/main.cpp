/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include <iostream>
#include <vector>
#include "hv/WebSocketServer.h"
#include "hv/EventLoop.h"
#include "GoProController.hpp"

std::vector<const WebSocketChannelPtr*> hosts = std::vector<const WebSocketChannelPtr*>();
GoProController controller;

void ExecuteCommand(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string target = "";
    json r = json::object();

    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }

    if(name == "reboot"){
        controller.reboot(target);
        channel->send(getPacket("command:reboot", r));
    }else if(name == "shutdown"){
        controller.shutdown(target);
        channel->send(getPacket("command:shutdown", r));
    }else if(name == "keep_alive"){
        controller.keep_alive(target);
        channel->send(getPacket("command:keep_alive", r));
    }else if(name == "usb_on"){
        controller.usb(target, true);
        channel->send(getPacket("command:usb_on", r));
    }else if(name == "usb_off"){
        controller.usb(target, false);
        channel->send(getPacket("command:usb_off", r));
    }else if(name == "datetime"){
        controller.datetime(target);
        channel->send(getPacket("command:datetime", r));
    }else if(name == "zoom"){
        controller.zoom(target);
        channel->send(getPacket("command:zoom", r));
    }else if(name == "shutter_on"){
        controller.shutter(target, true);
        channel->send(getPacket("command:shutter", r));
    }else if(name == "shutter_off"){
        controller.shutter(target, false);
        channel->send(getPacket("command:shutter_off", r));
    }
    else if(name == "ip"){
        r["data"] = json::parse(controller.getAllIP());
        channel->send(getPacket("command:ip", r));
    }
    else if(name == "scan"){
        controller.scanCameras();
        channel->send(getPacket("command:scan", r));
    }
    else if(name == "clean"){
        controller.cleanCameras();
        channel->send(getPacket("command:clean", r));
    }
    else if(name == "add" && target.size() >= 3){
        controller.addCameras(target);
        channel->send(getPacket("command:add", r));
    }else{
        channel->send(getPacket("command:unknown", r));
    }
}

void QueryAction(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string target = "";
    int id = 0;
    std::string value = "";
    json r = json::object();
    
    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["id"].is_number_integer()){
        id = j["id"].get<int>();
    }
    if(j["value"].is_string()){
        value = j["value"].get<std::string>();
    }

    if(name == "get"){
        r["data"] = json::parse(controller.queryStatus(target));
        channel->send(getPacket("query:get", r));
    }
    else if(name == "set"){
        r["data"] = json::parse(controller.setSetting(target, id, value));
        channel->send(getPacket("query:set", r));
    }
}

void WebcamAction(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string target = "";
    int port = 10000;
    int res = 4;
    int fov = 0;
    bool ts = true;
    json r = json::object();
    
    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["port"].is_number_integer()){
        port = j["port"].get<int>();
    }
    if(j["res"].is_number_integer()){
        res = j["res"].get<int>();
    }
    if(j["fov"].is_number_integer()){
        fov = j["fov"].get<int>();
    }
    if(j["ts"].is_boolean()){
        ts = j["ts"].get<bool>();
    }

    if(name == "preview"){
        controller.webcamMode(target);
        channel->send(getPacket("webcam:reboot", r));
    }
    else if (name == "exit"){
        controller.webcamUnMode(target);
        channel->send(getPacket("webcam:exit", r));
    }
    else if (name == "start"){
        controller.webcamOn(target, port, res, fov, ts);
        channel->send(getPacket("webcam:start", r));
    }
    else if (name == "stop"){
        controller.webcamOff(target);
        channel->send(getPacket("webcam:stop", r));
    }
    else if (name == "status"){
        r["data"] = json::parse(controller.webcamStatus(target));
        channel->send(getPacket("webcam:status", r));
    }
    else if (name == "version"){
        r["data"] = json::parse(controller.webcamVersion(target));
        channel->send(getPacket("webcam:version", r));
    }else{
        channel->send(getPacket("webcam:unknown", r));
    }
}

void ModeAction(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string target = "";
    int mode = 0;
    json r = json::object();

    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["mode"].is_number()){
        mode = j["mode"].get<int32_t>();
    }

    if(name == "load"){
        controller.setPreset(target, mode);
        channel->send(getPacket("preset:set", r));
    }
}

void MediaAction(const WebSocketChannelPtr& channel, json j){
    std::string target = "";
    
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }

}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    std::cout << "Starting GoPro Server (RPi)..." << std::endl;
    hv::WebSocketService ws;
    ws.onopen = [&](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        printf("Client connected: %s\n", channel->peeraddr().c_str());
        hosts.push_back(&channel);
    };
    ws.onmessage = [&](const WebSocketChannelPtr& channel, const std::string& msg) {
        std::thread([=]() {
            printf("Received: %s\n", msg.c_str());
            try{
                json j = json::parse(msg.c_str());
                // Simple command parsing
                if (j["key"].get<std::string>() == "command") {
                    ExecuteCommand(channel, j["value"]);
                }
                else if (j["key"].get<std::string>() == "query") {
                    QueryAction(channel, j["value"]);
                }
                else if (j["key"].get<std::string>() == "webcam") {
                    WebcamAction(channel, j["value"]);
                }
                else if (j["key"].get<std::string>() == "media") {
                    MediaAction(channel, j["value"]);
                }
            }catch(const std::exception& e){
                std::cerr << "JSON Parse error: " << e.what() << std::endl;
            }
        }).detach();
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

#ifdef ESP
extern "C" void app_main(){
    main();
}
#endif