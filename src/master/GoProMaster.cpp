/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "GoProMaster.h"
#include <iostream>

GoProMaster::GoProMaster() {
    t1 = std::thread(&GoProMaster::update, this);
    std::cout << "GoProMaster created" << std::endl;
}

GoProMaster::~GoProMaster() {
    std::cout << "GoProMaster destroy" << std::endl;
    setdone();
    if(t1.joinable()){
        t1.join();
    }
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
        std::thread([=]() {
            processMessage(conn->ip, msg);
        }).detach();
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

void GoProMaster::command_only(const std::string server, const std::string command, std::string target){
    json data = json::object();
    data["key"] = "command";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

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

void GoProMaster::query_only(const std::string server, const std::string command, std::string target){
    json data = json::object();
    data["key"] = "query";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

    for(auto s : servers){
        if(s->ip == server && s->connected){
            s->client.send(data.dump());
            break;
        }
    }
}

void GoProMaster::webcam_only(const std::string command, std::string target){
    json data = json::object();
    data["key"] = "webcam";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

    for(auto s : servers){
        if(s->connected){
            s->client.send(data.dump());
        }
    }
}

void GoProMaster::webcam_only(const std::string server, const std::string command, std::string target){
    json data = json::object();
    data["key"] = "webcam";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;

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

bool GoProMaster::applyAll(const std::string& ip, const json& res){
    for (auto& s : servers) {
        if (!s->connected) continue;
        for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
            int32_t id = GOPRO_SETTING_IDS[i];
            if(!res[std::to_string(id)].is_number()) continue;

            json get_status = json::object();
            get_status["key"] = "query";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "set";
            get_status["value"]["id"] = id;
            get_status["value"]["value"] = res[std::to_string(id)].get<int32_t>();
            s->client.send(get_status.dump());
        }
    }
}

void GoProMaster::registerCameraSettingFeedback(camera_setting_feedback v){
    _camera_setting_feedback = v;
}

void GoProMaster::registerCameraStatusFeedback(camera_status_feedback v){
    _camera_status_feedback = v;
}


const std::vector<std::shared_ptr<CameraInfo>>& GoProMaster::getCameras() const {
    return cameras;
}

const std::vector<std::shared_ptr<ServerConnection>>& GoProMaster::getServers() const {
    return servers;
}

void GoProMaster::update(){
    while (!done) {
        for (auto& s : servers) {
            if (!s->connected) continue;
            if (ipQueryFinish.count(s->ip) && ipQueryFinish.at(s->ip)) continue;
            json get_status = json::object();
            get_status["key"] = "command";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "ip";
            ipQueryFinish.insert_or_assign(s->ip, true);
            s->client.send(get_status.dump());
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void GoProMaster::processMessage(const std::string& server, const std::string& msg){
    try{
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
            cleanCameraFromServer(server);
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
                std::lock_guard<std::mutex> lock(camera_mtx);
                if(found == -1){
                    auto cam = std::make_shared<CameraInfo>();
                    cam->ip = ip_ref;
                    cam->server = server;
                    cameras.push_back(cam);
                }
            }
            ipQueryFinish.insert_or_assign(server, false);
        }
        else if(key == "query:get"){
            if(!data["value"]["data"].is_array()){
                std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
                std::cerr << "query:get, return value should be array" << std::endl;
                return;
            }
            /**
             * In case you're confuse here...
             * The data is like this
             * * data
             * * key
             * * value
             *   * data
             *     * ip <- Go pro ip address
             *     * status <- Where all the go pro status keeps
             */
            for(auto ip = data["value"]["data"].begin(); ip != data["value"]["data"].end(); ++ip){
                if(!ip.value()["ip"].is_string() || !ip.value()["status"].is_object()){
                    std::cerr << "query:get error: Require ip and status in value.data" << std::endl;
                    continue;
                }

                std::string ip_ref = ip.value()["ip"].get<std::string>();
                int32_t found = findCamera(ip_ref);
                CameraInfo _cam;
                if(found == -1){
                    auto cam = std::make_shared<CameraInfo>();
                    cam->state = ip.value()["status"];
                    _cam = *cam;
                    cameras.push_back(cam);
                    std::cout << "Added camera state " << ip_ref << std::endl;
                    std::cout << cam->state.dump() << std::endl;
                }else{
                    auto cam = cameras[found];
                    cam->state = ip.value()["status"];
                    _cam = *cam;
                    std::cout << "Update camera state " << ip_ref << std::endl;
                    std::cout << cam->state.dump() << std::endl;
                }
                if(_camera_setting_feedback != NULL){
                    json buffer_setting = json::object();
                    if(getSettingsFromCamera(_cam, buffer_setting)){
                        _camera_setting_feedback(buffer_setting);
                    }else{
                        std::cout << "Error setting feedback: getSettingsFromCamera failed" << std::endl;    
                    }
                }else{
                    std::cout << "Skip setting feedback: Detect function pointer is NULL" << std::endl;
                }
            }
        }
        else{
            std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
            std::cerr << "No registerd action from this key: " << key << std::endl;
        }
    }catch(const std::exception& e){
        std::cerr << "JSON Parse error: " << e.what() << std::endl;
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
    auto iter = std::find_if(cameras.begin(), cameras.end(),
        [&](auto &s){ return ((*s).server == ip); }
    );
    if(iter != cameras.end()){
        cameras.erase(iter);
        cleanCameraFromServer(ip);
    }
}

void GoProMaster::setdone(){
    done = true;
}

bool GoProMaster::getSettingsFromCamera(CameraInfo target, json& res){
    json data = target.state;
    if(!data["settings"].is_object()){
        std::cerr << "getSettingsFromCamera failed: Cannot get settings property" << std::endl;
        return false;
    }
    res = data["settings"];
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        int32_t id = GOPRO_SETTING_IDS[i];
        if(!res[std::to_string(id)].is_number()) continue;
        int32_t value = res[std::to_string(id)].get<int32_t>();

        const size_t size = GET_SETTING_SIZE_BY_ID(id);
        const int32_t* all_values = GET_SETTING_VALUE_BY_ID(id);
        for(int32_t j = 0; j < size; j++){
            if(all_values[j] == value){
                //res[std::to_string(id)] = j; // Set index, instead of value id
                break;
            }
        }
    }
    return true;
}

bool GoProMaster::getStatusFromCamera(CameraInfo target, json&& res){
    json data = target.state;
    if(!data["status"].is_object()){
        return false;
    }
    res = data["status"];
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