/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "GoProMaster.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

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
        std::lock_guard<std::mutex> lock(camera_mtx);
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
            std::lock_guard<std::mutex> lock(camera_mtx);
            cleanCameraFromServer(conn->ip);
            stateQueryFinish.insert_or_assign(conn->ip, false);
            ipQueryFinish.insert_or_assign(conn->ip, false);
            mediaQueryFinish.insert_or_assign(conn->ip, false);
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

void GoProMaster::command_with_value(const std::string command, std::string target, std::string value){
    json data = json::object();
    data["key"] = "command";
    data["value"] = json::object();
    data["value"]["name"] = command;
    data["value"]["target"] = target;
    data["value"]["value"] = value;

    for(auto s : servers){
        if(s->connected){
            s->client.send(data.dump());
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

void GoProMaster::webcam_start(const std::string server){

}

void GoProMaster::preview_start(std::string server, std::string target){
    json data = json::object();
    data["key"] = "preview";
    data["value"] = json::object();
    data["value"]["name"] = "start";
    data["value"]["target"] = target;

    for(auto s : servers){
        if(s->ip == server && s->connected){
            s->client.send(data.dump());
            break;
        }
    }
}

void GoProMaster::preview_end(std::string server, std::string target){
    json data = json::object();
    data["key"] = "preview";
    data["value"] = json::object();
    data["value"]["name"] = "stop";
    data["value"]["target"] = target;

    for(auto s : servers){
        if((s->ip == server || server.size() == 0) && s->connected){
            s->client.send(data.dump());
            break;
        }
    }
}

void GoProMaster::media_only(const std::string command, std::string target){

}

void GoProMaster::download_last_media(const std::string dir, bool put_finish){
    std::thread([=](){
        std::vector<std::string> urls = std::vector<std::string>();
        std::vector<std::string> names = std::vector<std::string>();
        for(auto& s : cameras){
            std::string filename = s->name + fs::path(s->last_media).extension().string();
            if(filename.size() == 0) continue;
            std::string _url = "http://" + s->server + ":8080/last_media?ip=" + s->ip;
            std::string _name = dir + "/" + filename;
            urls.push_back(_url);
            names.push_back(_name);
            std::cout << "media download: " << _url.c_str() << "  " << _name.c_str() << std::endl;
        }
        execs_download(urls, names);
        std::string finish_file = dir + "/" + "finish.txt";
        FILE* f = fopen(finish_file.c_str(), "wb");
        fclose(f);
    }).detach();
}

void GoProMaster::presetSwitch(const std::string server, const std::string target, int32_t mode) {
    std::thread([=](){
        for (auto& s : servers) {
            if (!s->connected) continue;
            if (server.size() > 0 && server != s->ip) continue;
            json get_status = json::object();
            get_status["key"] = "preset";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "load";
            get_status["value"]["mode"] = mode;
            get_status["value"]["target"] = target;
            s->client.send(get_status.dump());
        }
    }).detach();
}

void GoProMaster::locate(const std::string server, const std::string target){
    int32_t index = haslocate(server, target);
    if(index == -1){
        locates.push_back(std::pair<std::string, std::string>(server, target));
    }else{
        locates.erase(locates.begin() + index);
    }
}

int32_t GoProMaster::haslocate(const std::string server, const std::string target){
    for(int32_t i = 0; i < locates.size(); i++){
        auto& s = locates.at(i);
        if(s.first == server && s.second == target) return i;
    }
    return -1;
}

void GoProMaster::apply(const std::string& ip, const int32_t id, const int32_t value){
    std::thread([=](){    
        for (auto& s : servers) {
            if (!s->connected) continue;
            json get_status = json::object();
            get_status["key"] = "query";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "set";
            get_status["value"]["target"] = ip;
            get_status["value"]["id"] = id;
            get_status["value"]["value"] =  std::to_string(value);
            s->client.send(get_status.dump());
        }
    }).detach();
}

void GoProMaster::applyAll(const std::string& ip, const json& res){
    std::thread([=](){    
        for (auto& s : servers) {
            if (!s->connected) continue;
            json get_status = json::object();
            get_status["key"] = "query";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "setall";
            get_status["value"]["value"] = res;
            s->client.send(get_status.dump());
        }
    }).detach();
}

bool GoProMaster::directoryExists(const std::string& path) {
    if (fs::exists(path) && fs::is_directory(path)) {
        return true;
    }
    return false;
}

void GoProMaster::registerCameraSettingFeedback(camera_setting_feedback v){
    _camera_setting_feedback = v;
}

void GoProMaster::registerCameraStatusFeedback(camera_status_feedback v){
    _camera_status_feedback = v;
}

void GoProMaster::registerCameraLogFeedback(camera_log_feedback v){
    _camera_log_feedback = v;
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

        for (auto& s : servers) {
            if (!s->connected) continue;
            if (stateQueryFinish.count(s->ip) && stateQueryFinish.at(s->ip)) continue;
            json get_status = json::object();
            get_status["key"] = "query";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "getall";
            stateQueryFinish.insert_or_assign(s->ip, true);
            s->client.send(get_status.dump());
        }

        for (auto& s : servers) {
            if (!s->connected) continue;
            if (mediaQueryFinish.count(s->ip) && mediaQueryFinish.at(s->ip)) continue;
            json get_status = json::object();
            get_status["key"] = "media";
            get_status["value"] = json::object();
            get_status["value"]["name"] = "lastmedia";
            mediaQueryFinish.insert_or_assign(s->ip, true);
            s->client.send(get_status.dump());
        }

        std::lock_guard<std::mutex> lock(locate_mtx);
        for (auto& s : locates) {
            presetSwitch(s.first, s.second, 0);
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(3));
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
            if(!data["value"]["data"].is_array()){
                std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
                std::cerr << "command:ip, return value should be array" << std::endl;
                return;
            }
            std::vector<std::string> ips = std::vector<std::string>();
            std::vector<std::string> serial = std::vector<std::string>();
            std::unordered_map<std::string, std::string> names = std::unordered_map<std::string, std::string>();
            for(auto ip = data["value"]["data"].begin(); ip != data["value"]["data"].end(); ++ip){
                if(!ip.value().is_string()){
                    continue;
                }
                std::string ip_ref = ip.value().get<std::string>();
                
                std::vector<std::string> words = std::vector<std::string>();
                std::stringstream ss(ip_ref);
                std::string word;
                while (ss >> word) { // Extracts words separated by any whitespace
                    words.push_back(word);
                }

                if(words.size() > 0){
                    std::string serial_buffer = "";
                    serial_buffer += words[0][5];
                    serial_buffer += words[0][8];
                    serial_buffer += words[0][9];
                    serial.push_back(serial_buffer);
                    ips.push_back(words[0]);
                }
                if(words.size() > 1){
                    names.insert_or_assign(words[0], words[1]);
                }
            }
            std::lock_guard<std::mutex> lock(camera_mtx);
            replaceCameraFromServer(server, ips);

            for(int32_t i = 0; i < ips.size(); i++){
                int32_t index = findCamera(ips[i]);
                cameras[index]->serial = serial[i];
                if(names.count(ips[i])){
                    cameras[index]->name = names.at(ips[i]);
                }
            }

            ipQueryFinish.insert_or_assign(server, false);
        }
        else if(key == "query:get" || key == "query:getall"){
            if(key == "query:getall"){
                stateQueryFinish.insert_or_assign(server, false);
            }
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
            std::lock_guard<std::mutex> lock(camera_mtx);
            int32_t count = 0;

            // Clear the message
            if(key == "query:getall"){
                for(auto& camera : cameras){
                    if(camera->server == server){
                        camera->connected = false;
                    }
                }
            }

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
                    cam->connected = true;
                    _cam = *cam;
                    cameras.push_back(cam);
                    std::cout << "Added camera state " << ip_ref << std::endl;
                }else{
                    auto cam = cameras[found];
                    cam->state = ip.value()["status"];
                    cam->connected = cam->state["settings"]["2"].is_number_integer();
                    _cam = *cam;
                }

                if(_camera_setting_feedback != NULL){
                    json buffer_setting = json::object();
                    if(getSettingsFromCamera(_cam, buffer_setting)){
                        _camera_setting_feedback(_cam.ip, buffer_setting);
                    }
                }else{
                    std::cout << "Skip setting feedback: Detect function pointer is NULL" << std::endl;
                }
                if(_camera_status_feedback != NULL){
                    json buffer_status = json::object();
                    if(getStatusFromCamera(_cam, buffer_status)){
                        _camera_status_feedback(_cam.ip, buffer_status);
                    }
                }else{
                    std::cout << "Skip status feedback: Detect function pointer is NULL" << std::endl;
                }
                count++;
            }
        }
        else if(key == "query:set"){
            
        }
        else if(key == "media:lastmedia"){
            if(!data["value"]["data"].is_array()){
                std::cerr << "Invalid message from " << server << ": " << msg << std::endl;
                std::cerr << "media:lastmedia, return value should be array" << std::endl;
                return;
            }

            std::lock_guard<std::mutex> lock(camera_mtx);
            for(auto ip = data["value"]["data"].begin(); ip != data["value"]["data"].end(); ++ip){
                if(!ip.value()["ip"].is_string() || !ip.value()["filename"].is_object()){
                    std::cerr << "media:lastmedia error: Require ip and filename in value.data" << std::endl;
                    continue;
                }
                std::string folder = "";
                std::string file = "";
                if(ip.value()["filename"]["folder"].is_string()){
                    folder = ip.value()["filename"]["folder"].get<std::string>();
                }
                if(ip.value()["filename"]["file"].is_string()){
                    file = ip.value()["filename"]["folfileder"].get<std::string>();
                }
                std::string ip_ref = ip.value()["ip"].get<std::string>();
                int32_t found = findCamera(ip_ref);
                if(found == -1){
                    cameras[found]->last_media = folder + "/" + file;
                }
            }
            mediaQueryFinish.insert_or_assign(server, false);
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

void GoProMaster::cleanCameraFromServer(const std::string server){
    std::vector<std::shared_ptr<CameraInfo>>::iterator iter = std::find_if(cameras.begin(), cameras.end(),
        [&](auto &s){ return ((*s).server == server); }
    );

    while(iter != cameras.end()){
        cameras.erase(iter);
        iter = std::find_if(cameras.begin(), cameras.end(),
            [&](auto &s){ return ((*s).server == server); }
        );
    }
}

void GoProMaster::replaceCameraFromServer(const std::string server, const std::vector<std::string> ips){
    // Remove part
    auto it = std::remove_if(cameras.begin(), cameras.end(), [&](const std::shared_ptr<CameraInfo>& c) {
        // Only consider cameras belonging to this specific server
        if (c && c->server == server) {
            // If the camera's IP is NOT found in the new 'ips' list, delete it
            return std::find(ips.begin(), ips.end(), c->ip) == ips.end();
        }
        return false;
    });
    cameras.erase(it, cameras.end());

    // Append part
    for (const auto& new_ip : ips) {
        bool exists = false;
        for (const auto& existing_cam : cameras) {
            if (existing_cam && existing_cam->ip == new_ip) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            auto new_cam = std::make_shared<CameraInfo>();
            new_cam->server = server;
            new_cam->ip = new_ip;
            cameras.push_back(new_cam);
        }
    }
}

void GoProMaster::setdone(){
    done = true;
}

bool GoProMaster::getSettingsFromCamera(CameraInfo target, json& res){
    json data = target.state;
    if(!data["settings"].is_object()){
        return false;
    }
    res = data["settings"];
    // Convert value id to index number
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        int32_t id = GOPRO_SETTING_IDS[i];
        if(!res[std::to_string(id)].is_number()) continue;
        int32_t value = res[std::to_string(id)].get<int32_t>();

        const size_t size = GET_SETTING_SIZE_BY_ID(id);
        const int32_t* all_values = GET_SETTING_VALUE_BY_ID(id);
        for(int32_t j = 0; j < size; j++){
            if(all_values[j] == value){
                res[std::to_string(id)] = j; // Set index, instead of value id
                break;
            }
        }
    }
    return true;
}

bool GoProMaster::getStatusFromCamera(CameraInfo target, json& res){
    json data = target.state;
    if(!data["status"].is_object()){
        return false;
    }
    res = data["status"];
    // Convert value id to index number
    for(int32_t i = 0; i < GOPRO_STATUS_SIZE; i++){
        int32_t id = GOPRO_STATUS_IDS[i];
        const int32_t type = GET_STATUS_TYPE_BY_ID(id);
        if(type != (int32_t)CAMERA_STATUS_TYPE::OPTION){
            continue; // Only option type need to convert
        }
        if(!res[std::to_string(id)].is_number()) continue;
        int32_t value = res[std::to_string(id)].get<int32_t>();

        const size_t size = GET_STATUS_SIZE_BY_ID(id);
        const int32_t* all_values = GET_STATUS_VALUE_BY_ID(id);
        for(int32_t j = 0; j < size; j++){
            if(all_values[j] == value){
                res[std::to_string(id)] = j; // Set index, instead of value id
                break;
            }
        }
    }
    return true;
}

std::string GoProMaster::getBarInfo(const std::shared_ptr<CameraInfo> &c){
    json obj = c->state;
    bool find = false;
    std::string result = c->name + "  " + c->serial + "  " + c->ip + "  ";
    if(obj["settings"].is_object()){
        if(obj["settings"]["2"].is_number()){
            int32_t vr = obj["settings"]["2"].get<int32_t>();
            for(int32_t i = 0; i < VIDEO_RESOLUTION_SIZE; i++){
                if(vr == VIDEO_RESOLUTION_VALUE[i]){
                    vr = i;
                }
            }
            result += VIDEO_RESOLUTION_STRING[vr];
            result += "  ";
            find = true;
        }
        if(obj["settings"]["3"].is_number()){
            int32_t vr = obj["settings"]["3"].get<int32_t>();
            for(int32_t i = 0; i < FRAMES_PER_SECOND_SIZE; i++){
                if(vr == FRAMES_PER_SECOND_VALUE[i]){
                    vr = i;
                }
            }
            result += FRAMES_PER_SECOND_STRING[vr];
            result += "  ";
            find = true;
        }
    }
    if(!find) return c->ip + "  ...";
    return result;
}

int32_t GoProMaster::findCamera(const std::string ip){
    int32_t index = 0;    
    for(const auto& c : cameras){
        if(c){
            if(c->ip == ip){
                return index;
            }
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