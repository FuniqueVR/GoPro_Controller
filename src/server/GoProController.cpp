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
    camera_ips.clear();
    _updateRecord();
}

void GoProController::addCameras(std::string serial){
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
    if(target.size() > 0){
        _reboot(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_reboot(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::shutdown(std::string target){
    if(target.size() > 0){
        _shutdown(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_shutdown(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::keep_alive(std::string target){
    if(target.size() > 0){
        _keep_alive(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_keep_alive(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::usb(std::string target, bool ison){
    if(target.size() > 0){
        _usb(target, ison);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip, ison]() {
                this->_usb(ip, ison);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::datetime(std::string target){
    if(target.size() > 0){
        _datetime(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_datetime(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::zoom(std::string target){
    if(target.size() > 0){
        _zoom(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_zoom(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::shutter(std::string target, bool isstart){
    if(target.size() > 0){
        _shutter(target, isstart);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip, isstart]() {
                this->_shutter(ip, isstart);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

std::string GoProController::queryStatus(std::string target){
    json res;
    std::string address;
    json arr = json::array();
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
        json res;
        std::vector<std::pair<std::string, std::string>> results = _queryAllStatus(camera_ips);
        for(int32_t i = 0; i < results.size(); i++){
            address = results[i].first;
            res = json::parse(results[i].second);
            json b;
            b["ip"] = address;
            b["status"] = res;
            arr.push_back(b);
        }
    }
    return arr.dump();
}

std::string GoProController::setSetting(std::string target, int32_t ID, std::string value){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        // Target one IP
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
        std::cerr << "setSetting failed: target is empty" << std::endl;
    }
    return arr.dump();
}

std::string GoProController::setSettingAll(std::string target, json value){
    json res = json::array();
    for(int32_t i = 0; i < GOPRO_SETTING_SIZE; i++){
        int32_t id = GOPRO_SETTING_IDS[i];
        if(!value[std::to_string(id)].is_number()) {
            std::cerr << "setSettingAll failed: id "  << id << " is not number" << std::endl;
            continue;
        }
        int32_t val = value[std::to_string(id)].get<int32_t>();
        std::vector<std::future<std::string>> calls = 
            std::vector<std::future<std::string>>();
        for(std::string& ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip, id, val]() {
                return setSetting(ip, id, std::to_string(val));
            }));
        }

        for(auto& call : calls){
            call.wait();
            std::string result = call.get();
            std::cout << result << std::endl;
        }
        std::cout << "setSettingAll finish setting id: " << id << std::endl;
    }
    return res.dump();
}

void GoProController::webcamMode(std::string target){
    if(target.size() > 0){
        _webcamMode(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_webcamMode(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::webcamUnMode(std::string target){
    if(target.size() > 0){
        _webcamUnMode(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_webcamUnMode(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS){
    if(target.size() > 0){
        _webcamOn(target, startPort, res, fov, TS);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip, startPort, res, fov, TS]() {
                this->_webcamOn(ip, startPort, res, fov, TS);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
    }
}

void GoProController::webcamOff(std::string target){
    if(target.size() > 0){
        _webcamOff(target);
    }else{
        std::vector<std::future<void>> calls = std::vector<std::future<void>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                this->_webcamOff(ip);
            }));
        }

        for(auto& call : calls){
            call.get();
        }
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
