/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#ifdef _WIN32
#include "crashlogs.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#endif
#include <iostream>
#include <vector>
#include "../common/config.h"
#include "hv/WebSocketServer.h"
#include "hv/EventLoop.h"
#include "hv/UdpServer.h"
#include "hv/UdpClient.h"
#include "hv/hsocket.h"
#include "GoProController.h"

std::vector<const WebSocketChannelPtr*> hosts = std::vector<const WebSocketChannelPtr*>();
GoProController controller;

const int32_t listen_port = 8556;
const int32_t broadcast_port = 8554;

struct SenderStruct {
    std::string host_ip;
    std::string websocket_ip;
    struct sockaddr_in bcsa;
    int32_t sock_fd;
};

std::mutex broadcast_mtx;
std::vector<SenderStruct> broadcast_addrs = std::vector<SenderStruct>();

std::string getPacket(std::string key, json data){
    json response = json::object();
    response["key"] = key;
    response["value"] = data;
    return response.dump();
}

void ExecuteCommand(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string target = "";
    std::string value = "";
    int32_t ivalue = 0;
    json r = json::object();

    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["value"].is_string()){
        value = j["value"].get<std::string>();
    }
    if(j["value"].is_number_integer()){
        ivalue = j["value"].get<int32_t>();
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
        controller.zoom(target, ivalue);
        channel->send(getPacket("command:zoom", r));
    }else if(name == "shutter_on"){
        controller.shutter(target, true);
        channel->send(getPacket("command:shutter", r));
    }else if(name == "shutter_off"){
        controller.shutter(target, false);
        channel->send(getPacket("command:shutter_off", r));
    }else if(name == "ip"){
        r["data"] = json::parse(controller.getAllIP());
        channel->send(getPacket("command:ip", r));
    }else if(name == "locate_on"){
        controller.locate(target, true);
        r["data"] = json::object();
        channel->send(getPacket("command:locate_on", r));
    }else if(name == "locate_off"){
        controller.locate(target, false);
        r["data"] = json::object();
        channel->send(getPacket("command:locate_off", r));
    }else if(name == "model"){
        r["data"] = json::parse(controller.getAllModel());
        channel->send(getPacket("command:model", r));
    }else if(name == "scan"){
        controller.scanCameras();
        channel->send(getPacket("command:scan", r));
    }else if(name == "clean"){
        controller.cleanCameras();
        channel->send(getPacket("command:clean", r));
    }else if(name == "add" && target.size() >= 3){
        controller.addCameras(target);
        channel->send(getPacket("command:add", r));
    }
    else if(name == "delete" && target.size() >= 3){
        controller.deleteCameras(target);
        channel->send(getPacket("command:delete", r));
    }
    else if(name == "rename" && target.size() >= 3){
        controller.renameCameras(target, value);
        channel->send(getPacket("command:rename", r));
    }
    else{
        channel->send(getPacket("command:unknown", r));
    }
}

void QueryAction(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string source = "";
    std::string target = "";
    int id = 0;
    int preset = 0;
    std::string value = "";
    json jvalue = json::object();
    json r = json::object();
    
    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["source"].is_string()){
        source = j["source"].get<std::string>();
    }
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["id"].is_number_integer()){
        id = j["id"].get<int>();
    }
    if(j["preset"].is_number_integer()){
        preset = j["preset"].get<int>();
    }
    if(j["value"].is_string()){
        value = j["value"].get<std::string>();
    }
    else if(j["value"].is_object()){
        jvalue = j["value"];
    }

    // The reason we need to seperate the set and setall
    // It's because we needs to know which one is called by master update loop
    // And which one is called by UI event
    //
    // We don't want to flip the update flag when it's actually the UI event...
    if(name == "get"){
        r["data"] = json::parse(controller.queryStatus(target));
        channel->send(getPacket("query:get", r));
    }
    else if(name == "getall"){
        r["data"] = json::parse(controller.queryStatus(""));
        channel->send(getPacket("query:getall", r));
    }
    else if(name == "set"){
        r["data"] = json::parse(controller.setSetting(target, id, value));
        channel->send(getPacket("query:set", r));
    }
    else if(name == "setall"){
        r["data"] = json::parse(controller.setSettingAll(source, target, preset, jvalue));
        channel->send(getPacket("query:setall", r));
    }
    else{
        channel->send(getPacket("query:unknown", r));
    }
}

void WebcamAction(const WebSocketChannelPtr& channel, json j){
    std::string name = "";
    std::string target = "";
    int port = 8554;
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
    }else{
        channel->send(getPacket("webcam:unknown", r));
    }
}

void MediaAction(const WebSocketChannelPtr& channel, json j){
    std::string target = "";
    std::string name = "";
    json r = json::object();
    
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }

    if(name == "lastmedia"){
        controller.getMediaList(target);
        channel->send(getPacket("media:lastmedia", r));
    }else{
        channel->send(getPacket("media:unknown", r));
    }
}

void PreviewAction(const WebSocketChannelPtr& channel, json j){
    std::string target = "";
    std::string name = "";
    int32_t port = 8556;
    json r = json::object();
    
    if(j["target"].is_string()){
        target = j["target"].get<std::string>();
    }
    if(j["name"].is_string()){
        name = j["name"].get<std::string>();
    }
    if(j["port"].is_number_integer()){
        port = j["port"].get<int32_t>();
    }

    if(name == "start"){
        controller.previewOn(target, port);
        channel->send(getPacket("preview:start", r));
    }else if (name == "stop"){
        controller.previewOff(target);
        channel->send(getPacket("preview:stop", r));
    }
    else{
        channel->send(getPacket("preview:unknown", r));
    }
}

void WebsocketServer(){
    std::cout << "Starting GoPro Server (RPi)..." << std::endl;
    hv::WebSocketService ws;
    ws.onopen = [&](const WebSocketChannelPtr& channel, const HttpRequestPtr& req) {
        std::lock_guard<std::mutex> lock(broadcast_mtx);
        printf("Client connected: %s\n", channel->peeraddr().c_str());
        hosts.push_back(&channel);

        int32_t f = -1;
        for(int32_t i = 0; i < broadcast_addrs.size(); i++){
            if(broadcast_addrs.at(i).websocket_ip == channel->peeraddr().c_str()){
                f = i;
                break;
            }
        }

        if(f == -1){
            SenderStruct sss = SenderStruct();
            std::string addd = channel->peeraddr().c_str();
            while(addd.at(addd.size() - 1) != ':'){
                addd.pop_back();
            }
            addd.pop_back();

            sss.websocket_ip = channel->peeraddr().c_str();
            sss.host_ip = addd.c_str();

            sss.sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
            memset(&sss.bcsa, 0, sizeof(sss.bcsa));
            sss.bcsa.sin_family = AF_INET;
            sss.bcsa.sin_port = htons(broadcast_port);
            sss.bcsa.sin_addr.s_addr = inet_addr(addd.c_str());
        
            broadcast_addrs.push_back((sss));
        }
    };
    ws.onmessage = [&](const WebSocketChannelPtr& channel, const std::string& msg) {
        std::thread([=]() {
            if(SERVER_QUERY_LOG) printf("Received: %s\n", msg.c_str());
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
                else if (j["key"].get<std::string>() == "preview") {
                    PreviewAction(channel, j["value"]);
                }
                else if (j["key"].get<std::string>() == "preset") {
                    ModeAction(channel, j["value"]);
                }
            }catch(const std::exception& e){
                std::cerr << "JSON Parse error: " << e.what() << std::endl;
            }
        }).detach();
    };
    ws.onclose = [&](const WebSocketChannelPtr& channel) {
        std::lock_guard<std::mutex> lock(broadcast_mtx);
        printf("Client disconnected: %s\n", channel->peeraddr().c_str());
        for(int32_t i = 0; i < hosts.size(); i++){
            bool find = std::strcmp(hosts[i]->get()->peeraddr().c_str(),
            channel->peeraddr().c_str());
            if(find){
                hosts.erase(hosts.begin() + i);
                break;
            }
        }
        
        int32_t f = -1;
        for(int32_t i = 0; i < broadcast_addrs.size(); i++){
            if(broadcast_addrs.at(i).websocket_ip == channel->peeraddr().c_str()){
                f = i;
                break;
            }
        }
        if(f >= 0){
            SenderStruct& sss = broadcast_addrs.at(f);
            closesocket(sss.sock_fd);
            broadcast_addrs.erase(broadcast_addrs.begin() + f);
        }
    };

    hv::WebSocketServer server;
    server.registerWebSocketService(&ws);
    server.setPort(9090);

    std::cout << "WebSocket Server listening on port 9090..." << std::endl;
    server.run();
}

void HttpServer(){
    hv::HttpService router;
    if(fs::exists("res")){
        fs::remove_all("res");
    }
    fs::create_directory("res");
    router.Static("/res", "./res");
    router.GET("/last_media", [](HttpRequest* req, HttpResponse* resp) {
        std::string target_ip = req->GetParam("ip");
        std::string is_local_str = req->GetParam("local");
        bool is_local = is_local_str == "1";

        std::cout << "Http GET /last_media " << target_ip << std::endl;

        if (target_ip.empty()) {
            resp->status_code = http_status::HTTP_STATUS_BAD_REQUEST;
            return resp->String("{\"error\": \"Missing ip parameter\"}");
        }

        try{
            std::string res = exec("http://" + target_ip + ":8080/gopro/media/last_captured");
            if(res.size() == 0) {
                return resp->String("{\"error\": \"IP fetch failed\"}");
            }
            json last_data = json::parse(res);
            if(!last_data["file"].is_string() || !last_data["folder"].is_string()){
                resp->status_code = http_status::HTTP_STATUS_BAD_REQUEST;
                return resp->String("{\"error\": \"no last media file\"}");
            }
            std::string folder = last_data["folder"].get<std::string>();
            std::string file = last_data["file"].get<std::string>();

            std::string gopro_url = "http://" + target_ip + ":8080/videos/DCIM/" + folder + "/" + file + "?download=true";

            if(is_local){
                return resp->String("{\"path\": \"" + gopro_url + "\"}");
            }else{
                int32_t t = 0;
                std::string download_path = "temp.download";
                while(fs::exists("res/" + download_path)){
                    download_path = "temp.download" + std::to_string(t);
                    t++;
                }
                size_t size = requests::downloadFile(gopro_url.c_str(), ("res/" + download_path).c_str(), [&target_ip](size_t received_bytes, size_t total_bytes){
                    std::cout << "[last_media] download " << target_ip << " " << received_bytes << " / " << total_bytes << std::endl;
                });
                return resp->String("{\"path\": \"" + download_path + "\"}");
            }
        }
        catch(const std::exception& ex){
            std::cerr << ex.what() << std::endl;
        }
    });

    hv::HttpServer http_server;
    http_server.registerHttpService(&router);
    http_server.setPort(8080);
    http_server.setThreadNum(4);
    
    std::cout << "Http Server listening on port 8080..." << std::endl;

    http_server.run();
}

void UDPProxyServer(){
    std::cout << "Starting GoPro UDP Proxy Server (RPi)..." << std::endl;
    static hv::UdpServer us;
    int32_t bindfd = us.createsocket(listen_port);
    if(bindfd == -1){
        std::cerr << "Failed to create socket for recevier: " << std::endl;
        return;
    }
    std::cout << "UDP bind on port: " << listen_port << std::endl;
    std::cout << "UDP Broadcast Relay started:" << std::endl;
    std::cout << "  Listening on: 0.0.0.0:" << listen_port << " (from GoPro)" << std::endl;
    std::cout << "  Broadcasting to: " << broadcast_port << " (to all Masters)" << std::endl;

    us.onMessage = [&](const hv::SocketChannelPtr& channel, hv::Buffer* buf){
        std::lock_guard<std::mutex> lock(broadcast_mtx);
        for(auto& sss : broadcast_addrs){
#ifdef _WIN32
            sendto(sss.sock_fd, (const char*)buf->data(), buf->size(), 0,
                (struct sockaddr*)&sss.bcsa, 
                sizeof(sss.bcsa));
#else
            sendto(sss.sock_fd, buf->data(), buf->size(), 0,
                (struct sockaddr*)&sss.bcsa, 
                sizeof(sss.bcsa));
#endif
        }
    };
    us.start();
}

int main() {
#ifdef _WIN32
    glaiel::crashlogs::set_crashlog_folder(".");
    glaiel::crashlogs::set_crashlog_filename("crashlog.txt");
    glaiel::crashlogs::set_on_write_crashlog_callback([](std::string msg){
        std::cerr << "Crash: " << msg.c_str() << std::endl;
    });
    glaiel::crashlogs::begin_monitoring();
#endif
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    std::thread t1 = std::thread([=]() {
        WebsocketServer();
    });

    std::thread t2 = std::thread([=]() {
        HttpServer();
    });

    std::thread t3 = std::thread([=]() {
        UDPProxyServer();
    });

    controller.update();

    t3.join();
    t2.join();
    t1.join();
    return 0;
}

#ifdef ESP
extern "C" void app_main(){
    main();
}
#endif