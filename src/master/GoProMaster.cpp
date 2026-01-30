#include "GoProMaster.h"
#include "../common/iphelper.h"
#include <iostream>

GoProMaster::GoProMaster() {
    t1 = std::thread(&GoProMaster::update, this);
    std::cout << "GoProMaster created" << std::endl;
}

GoProMaster::~GoProMaster() {
    std::cout << "GoProMaster destroy" << std::endl;
    for (auto& s : servers) {
        s->client.close();
        cleanCameraFromServer(s->ip);
    }
}

std::string GoProMaster::addServer(const std::string& ip) {
    for(auto & s : servers){
        if(s->ip == ip){
            std::cout << "Server " << ip << " already exists." << std::endl;
            return s->ip;
        }
    }

    auto conn = std::make_shared<ServerConnection>();
    conn->ip = ip;
    
    conn->client.onopen = [conn, this]() {
        std::cout << "Connected to server: " << conn->ip << std::endl;
        conn->connected = true;
    };
    conn->client.onmessage = [conn, this](const std::string& msg) {
        processMessage(conn->ip, msg);
    };
    conn->client.onclose = [conn, this]() {
        if(conn->connected){
            std::cout << "Disconnected from server: " << conn->ip << std::endl;
            conn->connected = false;
            cleanCameraFromServer(conn->ip);
        }
    };

    servers.push_back(conn);
    return conn->ip;
}

void GoProMaster::reconnectAll() {
    for (auto& s : servers) {
        if (!s->connected) {
            std::cout << "Connecting to " << s->ip << "..." << std::endl;
            // Assuming ws://ip:9090 based on server implementation
            std::string url = "ws://" + s->ip + ":9090";
            s->client.open(url.c_str());
        }
    }
}

void GoProMaster::disconnectAll(){
    for (auto& s : servers) {
        if (s->connected) {
            std::cout << "Disconnect to " << s->ip << "..." << std::endl;
            s->client.close();
        }
    }
}

void GoProMaster::cleanAll(){
    for(int i = servers.size() - 1; i >=0; --i){
        auto& s = servers[i];
        if(!s->connected){
            std::cout << "Cleaning " << s->ip << "..." << std::endl;
            servers.erase(servers.begin() + i);
        }
    }
}

void GoProMaster::reconnect(const std::string& ip){
    for(auto& s : servers){
        if(s->ip == ip && !s->connected){
            std::cout << "Connecting to " << s->ip << "..." << std::endl;
            std::string url = "ws://" + s->ip + ":9090";
            s->client.open(url.c_str());
            break;
        }
    }
}

void GoProMaster::disconnect(const std::string& ip){
    for(auto& s : servers){
        if(s->ip == ip && s->connected){
            std::cout << "Disconnecting to " << s->ip << "..." << std::endl;
            s->client.close();
            break;
        }
    }
}

void GoProMaster::clean(const std::string& ip){
    for(int i = servers.size() - 1; i >=0; --i){
        auto& s = servers[i];
        if(s->ip == ip && !s->connected){
            std::cout << "Cleaning " << s->ip << "..." << std::endl;
            servers.erase(servers.begin() + i);
        }
    }
}

void GoProMaster::command_only(const std::string command, std::string target){
    json data = json::object();
    data["key"] = "command";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

    for(auto s : servers){
        if(s->connected){
            s->client.send(data.dump());
        }
    }
}

void GoProMaster::command_only(const std::string camera, const std::string command, std::string target){
    json data = json::object();
    data["key"] = "command";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

    int32_t cam_index = findCamera(camera);
    std::string server = "";
    bool findserver = false;
    for(auto c : cameras){
        if(c->ip == camera && c->server != ""){
            findserver = true;
            server = c->server;
            break;
        }
    }
    if(!findserver) return;
    for(auto s : servers){
        if(s->ip == server && s->connected){
            s->client.send(data.dump());
            break;
        }
    }
}

void GoProMaster::query_only(const std::string command, std::string target){
    json data = json::object();
    data["key"] = "query";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

    for(auto s : servers){
        if(s->connected){
            s->client.send(data.dump());
        }
    }
}

void GoProMaster::query_only(const std::string camera, const std::string command, std::string target){
    json data = json::object();
    data["key"] = "query";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

    int32_t cam_index = findCamera(camera);
    std::string server = "";
    bool findserver = false;
    for(auto c : cameras){
        if(c->ip == camera && c->server != ""){
            findserver = true;
            server = c->server;
            break;
        }
    }
    if(!findserver) return;
    for(auto s : servers){
        if(s->ip == server && s->connected){
            s->client.send(data.dump());
            break;
        }
    }
}

void GoProMaster::startRecordingAll() {
    sendToAll("f2_start_recording");
}

void GoProMaster::stopRecordingAll() {
    sendToAll("f3_stop_recording");
}

void GoProMaster::setModePhotoAll() {
    sendToAll("f4_mode_photo");
}

void GoProMaster::setModeVideoAll() {
    sendToAll("f5_mode_video");
}

const std::vector<std::shared_ptr<CameraInfo>>& GoProMaster::getCameras() const {
    return cameras;
}

const std::vector<std::shared_ptr<ServerConnection>>& GoProMaster::getServers() const {
    return servers;
}

void GoProMaster::update(){
    while (true) {
        for (auto& s : servers) {
            if (!s->connected) continue;
            json get_status = json::object();
            get_status["key"] = "command";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "ip";
            s->client.send(get_status.dump());
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void GoProMaster::processMessage(const std::string& server, const std::string& msg){
    json data = json::parse(msg);
    if(!data["key"].is_string()){
        std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
        std::cerr << "Key is not string" << std::endl;
        return;
    }
    if(data["value"].is_null()){
        std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
        std::cerr << "Value is null" << std::endl;
        return;
    }
    std::string key = data["key"].get<std::string>();
    if(key == "command:ip"){
        if(!data["value"]["data"].is_array()){
            std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
            std::cerr << "command:ip, return value should be array" << std::endl;
            return;
        }
        for(auto ip = data["value"]["data"].begin(); ip != data["value"]["data"].end(); ++ip){
            if(!ip.value().is_string()){
                continue;
            }
            std::string ip_ref = ip.value().get<std::string>();
            int32_t found = findCamera(ip_ref);
            if(found == -1){
                std::lock_guard<std::mutex> lock(server_mtx);
                auto cam = std::make_shared<CameraInfo>();
                cam->ip = ip_ref;
                cam->server = server;
                cameras.push_back(cam);
                std::cout << "Discovered camera " << ip_ref << " from server " << server << std::endl;
            }
        }
    }
    else if(key == "query:get"){
        if(!data["value"]["data"].is_array()){
            std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
            std::cerr << "query:get, return value should be array" << std::endl;
            return;
        }
        for(auto ip = data["value"]["data"].begin(); ip != data["value"]["data"].end(); ++ip){
            if(!ip.value()["ip"].is_string() || !ip.value()["status"].is_object()){
                continue;
            }

            std::string ip_ref = ip.value()["ip"].get<std::string>();
            int32_t found = findCamera(ip_ref);
            if(found == -1){
                auto cam = std::make_shared<CameraInfo>();
                cam->state = ip.value()["status"];
                cameras.push_back(cam);
            }
        }
    }
    else{
        std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
        std::cerr << "No registerd action from this key: " << key << std::endl;
    }
}

void GoProMaster::sendToAll(const std::string& msg) {
    for (auto& s : servers) {
        if (s->connected) {
            s->client.send(msg);
        }
    }
}

void GoProMaster::cleanCameraFromServer(const std::string ip){
    for(std::vector<std::shared_ptr<CameraInfo>>::iterator i = cameras.end() + 1; i >= cameras.begin(); i--){
        const auto& c = i->get();
        if(c){
            if(c->server == ip){
                std::lock_guard<std::mutex> lock(server_mtx);
                cameras.erase(i);
            }
        }
    }
}

bool GoProMaster::getStateFromCamera(CameraInfo target, ConvertSetting&& res){
    json data = target.state;
    if(data["settings"].is_object()){
        if(data["settings"].at(2).is_number()){
            res.resolution = data["settings"].at(2).get<int32_t>();
        }
        if(data["settings"].at(3).is_number()){
            res.fps = data["settings"].at(3).get<int32_t>();
        }
    }
    return true;
}

int32_t GoProMaster::findCamera(const std::string ip){
    int32_t index = 0;
    for(const auto& c : cameras){
        if(c->ip == ip){
            return index;
        }
        ++index;
    }
    return -1;
}

int32_t GoProMaster::findServer(const std::string ip){
    int32_t index = 0;
    for(const auto& c : servers){
        if(c->ip == ip){
            return index;
        }
        ++index;
    }
    return -1;
}

int32_t GoProMaster::findCamera(const std::string server, const std::string ip){
    int32_t index = 0;
    for(const auto& c : cameras){
        if(c->server == server && c->ip == ip){
            return index;
        }
        ++index;
    }
    return -1;
}