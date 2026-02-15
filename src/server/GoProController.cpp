/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "GoProController.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>   // For std::chrono::seconds, milliseconds, etc.
#include <thread>   // For std::this_thread::sleep_for
#include <future>
#include <unistd.h>
#include <sys/types.h>

std::string getPacket(std::string key, json data){
    json response = json::object();
    response["key"] = key;
    response["value"] = data;
    return response.dump();
}

GoProController::GoProController() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    _loadRecord();
}

GoProController::~GoProController() {
    curl_global_cleanup();
    for(auto& thread : scan_workers){
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void GoProController::scanCameras() {
    if(!mdns_scaned){
        mdns_scaned = true;
            mdns_cpp::Logger::setLoggerSink([&](const std::string& log_msg) {
            std::string keyword = std::string("gopro");
            size_t pos = log_msg.find(keyword);
            if(pos != std::string::npos){
                std::string p = std::string(log_msg.substr(0,13).c_str());
                std::cout << "[MDNS] gopro service found: " << p << "\n";
                bool find = false;
                for(auto i : camera_ips){
                    if(i == p){
                        find = true;
                        break;
                    }
                }
                if(!find){
                    std::lock_guard<std::mutex> lock(ips_mutex);
                    camera_ips.push_back(p);
                    _updateRecord();
                }
            }else {
                std::cout << "[MDNS] ignore service: " << log_msg << "\n";
            }
        });
    }
    scan_workers.push_back(std::thread([&](){
        mdns.executeDiscovery();
    }));
}

void GoProController::cleanCameras(){
    std::lock_guard<std::mutex> lock(ips_mutex);
    camera_ips.clear();
    _updateRecord();
}

void GoProController::addCameras(std::string serial){
    std::lock_guard<std::mutex> lock(ips_mutex);
    if(serial.size() >= 3){
        std::string p = GetRemoteIPBySerial(serial);
        bool find = false;
        for(auto i : camera_ips){
            if(i == p){
                find = true;
                break;
            }
        }
        if(!find){
            camera_ips.push_back(p);
            _updateRecord();
        }
    }
}

void GoProController::deleteCameras(std::string ip) {
    std::lock_guard<std::mutex> lock(ips_mutex);
    for(int32_t i = 0; i < camera_ips.size(); i++){
        if(camera_ips[i] == ip){
            camera_ips.erase(camera_ips.begin() + i);
            return;
        }
    }
}

void GoProController::renameCameras(std::string ip, std::string name){
    camera_name.insert_or_assign(ip, name);
    _updateRecord();
}

void GoProController::setPreset(std::string target, int32_t mode){
    if(target.size() > 0){
        _setPreset(target, mode);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip, mode]() {
                this->_setPreset(ip, mode);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::reboot(std::string target){
    if(target.size() > 0) _reboot(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _rebootAll(camera_ips);
    }
}

void GoProController::shutdown(std::string target){
    if(target.size() > 0) _shutdown(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _shutdownAll(camera_ips);
    }
}

void GoProController::keep_alive(std::string target){
    if(target.size() > 0) _keepAlive(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _keepAliveAll(camera_ips);
    }
}

void GoProController::usb(std::string target, bool ison){
    if(target.size() > 0) _usb(target, ison); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _usbAll(camera_ips, ison);
    }
}

void GoProController::datetime(std::string target){
    if(target.size() > 0) _datetime(target);
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _datetimeAll(camera_ips);
    }
}

void GoProController::zoom(std::string target, int32_t value){
    if(target.size() > 0) _zoom(target, value); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _zoomAll(camera_ips, value);
    }
}

void GoProController::shutter(std::string target, bool isstart){
    if(target.size() > 0) _shutter(target, isstart); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _shutterAll(camera_ips, isstart);
    }
}

std::string GoProController::queryStatus(std::string target){
    json arr = json::array();
    json res = json::object();
    std::string address;
    if(target.size() > 0){
        try{
            std::pair<std::string, std::string> result = _queryStatus(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::lock_guard<std::mutex> lock(ips_mutex);
        std::vector<std::pair<std::string, std::string>> results = _queryAllStatus(camera_ips);
        for(int32_t i = 0; i < results.size(); i++){
            try{
                address = results[i].first;
                res = json::parse(results[i].second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json j;
            j["ip"] = address;
            j["status"] = res;
            arr.push_back(j);
        }
    }
    return arr.dump();
}

std::string GoProController::setSetting(std::string target, int32_t ID, std::string value){
    json arr = json::array();
    json res = json::object();
    std::string address;
    if(target.size() > 0){
        try{
            std::pair<std::string, std::string> result = _setSetting(target, ID, value);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            std::cerr << "setSetting failed: "  << ex.what() << std::endl;
            res = json::object();
        }
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::lock_guard<std::mutex> lock(ips_mutex);
        std::vector<std::pair<std::string, std::string>> results = _setAllSetting(camera_ips, ID, value);
        for(int32_t i = 0; i < results.size(); i++){
            try{
                address = results[i].first;
                res = json::parse(results[i].second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json j;
            j["ip"] = address;
            j["status"] = res;
            arr.push_back(j);
        }
    }
    return arr.dump();
}

std::string GoProController::setSettingAll(std::string target, json value){
    return "{}";
}

void GoProController::webcamMode(std::string target){
    if(target.size() > 0) _webcamMode(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _webcamAllMode(camera_ips);
    }
}

void GoProController::webcamUnMode(std::string target){
    if(target.size() > 0) _webcamUnMode(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _webcamAllUnMode(camera_ips);
    }
}

void GoProController::webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS){
    if(target.size() > 0) _webcamOn(target, startPort, res, fov, TS); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _webcamAllOn(camera_ips, startPort, res, fov, TS);
    }
}

void GoProController::webcamOff(std::string target){
    if(target.size() > 0) _webcamOff(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_mutex);
        _webcamAllOff(camera_ips);
    }
}

std::string GoProController::webcamStatus(std::string target){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        json res;
        try{
            std::pair<std::string, std::string> result = _webcamStatus(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::vector<std::future<std::pair<std::string, std::string>>> calls = 
            std::vector<std::future<std::pair<std::string, std::string>>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                return _webcamStatus(ip);
            }));
        }

        for(auto& call : calls){
            try{
                std::pair<std::string, std::string> result = call.get();
                address = result.first;
                res = json::parse(result.second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json i;
            i["ip"] = address;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    return arr.dump();
}

std::string GoProController::webcamVersion(std::string target){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        json res;
        try{
            std::pair<std::string, std::string> result = _webcamVersion(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::vector<std::future<std::pair<std::string, std::string>>> calls = 
            std::vector<std::future<std::pair<std::string, std::string>>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                return _webcamVersion(ip);
            }));
        }

        for(auto& call : calls){
            try{
                std::pair<std::string, std::string> result = call.get();
                address = result.first;
                res = json::parse(result.second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json i;
            i["ip"] = address;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    return arr.dump();
}

void GoProController::previewOn(std::string target, int32_t port){
    if(target.size() > 0) _previewOn(target, port); 
    else _previewAllOn(camera_ips, port);
}

void GoProController::previewOff(std::string target){
    if(target.size() > 0) _previewOff(target); 
    else _previewAllOff(camera_ips);
}

std::string GoProController::getMediaList(std::string target){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        json res;
        try{
            std::pair<std::string, std::string> result = _getMediaList(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::vector<std::future<std::pair<std::string, std::string>>> calls = 
            std::vector<std::future<std::pair<std::string, std::string>>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                return _getMediaList(ip);
            }));
        }

        for(auto& call : calls){
            try{
                std::pair<std::string, std::string> result = call.get();
                address = result.first;
                res = json::parse(result.second);
            }catch(const std::exception& ex){
                res = json::object();
            }
            json i;
            i["ip"] = address;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    return arr.dump();
}

std::string GoProController::getLastMedia(std::string target){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        try{
            std::pair<std::string, std::string> result = _getLastMedia(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        json i;
        i["ip"] = address;
        i["filename"] = res;
        arr.push_back(i);
    }else{
        json res;
        std::vector<std::pair<std::string, std::string>> results = _getAllLastMedia(camera_ips);
        for(int32_t i = 0; i < results.size(); i++){
            address = results[i].first;
            res = json::parse(results[i].second);
            json b;
            b["ip"] = address;
            b["filename"] = res;
            arr.push_back(b);
        }
    }
    return arr.dump();
}

std::string GoProController::getAllIP(){
    json result = json::array();
    for(std::string target : camera_ips){
        if(camera_name.count(target)){
            result.push_back(target + " " + camera_name.at(target));
        }else{
            result.push_back(target);
        }
    }
    return result.dump();
}
