/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "GoProController.hpp"
#include "../common/iphelper.h"
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

std::string getCommand(std::string url){
    return url;
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
        json res;
        try{
            std::pair<std::string, std::string> result = _queryStatus(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(...){
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
            calls.push_back(std::async(std::launch::async, [this, ip, arr]() {
                return _queryStatus(ip);
            }));
        }

        for(auto& call : calls){
            try{
                std::pair<std::string, std::string> result = call.get();
                address = result.first;
                res = json::parse(result.second);
            }catch(...){
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

std::string GoProController::setSetting(std::string target, int ID, std::string value){
    json res;
    std::string address;
    json arr = json::array();
    if(target.size() > 0){
        try{
            std::pair<std::string, std::string> result = _setSetting(target, ID, value);
            address = result.first;
            res = json::parse(result.second);
        }catch(...){
            res = json::object();
        }
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::vector<std::future<std::pair<std::string, std::string>>> calls = 
            std::vector<std::future<std::pair<std::string, std::string>>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip, ID, value]() {
                return _setSetting(ip, ID, value);
            }));
        }

        for(auto& call : calls){
            try{
                std::pair<std::string, std::string> result = call.get();
                address = result.first;
                res = json::parse(result.second);
            }catch(...){
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

void GoProController::webcamOn(std::string target, int startPort, int res, int fov, bool TS){
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
        }catch(...){
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
            }catch(...){
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
        }catch(...){
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
            }catch(...){
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
        }catch(...){
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
            }catch(...){
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

std::string GoProController::getAllIP(){
    json result = json::array();
    for(std::string target : camera_ips){
        result.push_back(target);
    }
    return result.dump();
}

void GoProController::_loadRecord(){
    std::string homedir = get_env_var("WS_ROOT");
    homedir += "/record.txt";
    std::cout << "Trying load data from: " << homedir << std::endl;
    std::ifstream inFile(homedir.c_str());
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open the file: " << homedir << std::endl;
        return; // Return with an error code
    }
    std::string line;
    while (std::getline(inFile, line)) {
        camera_ips.push_back(line);
    }
    inFile.close();
}

void GoProController::_updateRecord(){
    std::string homedir = get_env_var("WS_ROOT");
    homedir += "/record.txt";
    std::cout << "Trying export data to: " << homedir << std::endl;
    std::ofstream outFile( homedir.c_str() );
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open the file." << homedir << std::endl;
        return; // Return with an error code
    }
    for(auto i : camera_ips){
        outFile << i << "\n";
    }
    outFile.close();
}

void GoProController::_setPreset(std::string target, int32_t mode){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/presets/load?id=" + std::to_string(mode);
    std::cout << "Set preset: " << url << std::endl;
    exec(getCommand(url));
}

void GoProController::_reboot(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gp/gpControl/command/system/reset";
    exec(getCommand(url));
}

void GoProController::_shutdown(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gp/gpControl/command/system/shutdown";
    exec(getCommand(url));
}

void GoProController::_keep_alive(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/keep_alive";
    exec(getCommand(url));
}

void GoProController::_usb(std::string target, bool ison){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/control/wired_usb?p=";
    if(ison) url += "1";
    else url += "0";
    exec(getCommand(url));
}

void GoProController::_datetime(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/control/wired_usb?p=0";
    exec(getCommand(url));
}

void GoProController::_zoom(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/control/wired_usb?p=0";
    exec(getCommand(url));
}

void GoProController::_shutter(std::string target, bool ison){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/shutter/";
    if(ison) url += "start";
    else url += "stop";
    exec(getCommand(url));
}

std::pair<std::string, std::string> GoProController::_queryStatus(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/state";
    return std::pair<std::string, std::string>(target, exec(getCommand(url)));
}

std::pair<std::string, std::string> GoProController::_setSetting(std::string target, int ID, std::string value){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return std::pair<std::string, std::string>(target, exec(getCommand(url)));
}

void GoProController::_webcamMode(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/preview";
    exec(getCommand(url));
}

void GoProController::_webcamUnMode(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/exit";
    exec(getCommand(url));
}

void GoProController::_webcamOn(std::string target, int startPort, int res, int fov, bool TS){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/start?res=";
    url += std::to_string(res);
    url += "&fov=";
    url += std::to_string(fov);
    url += "&port=";
    url += std::to_string(startPort);
    if(TS){
        url += "&protocol=TS";
    }else{
        url += "&protocol=RTSP";
    }
    exec(getCommand(url));
}

void GoProController::_webcamOff(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/stop";
    exec(getCommand(url));
}

std::pair<std::string, std::string> GoProController::_webcamStatus(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/status";
    return std::pair<std::string, std::string>(target, exec(getCommand(url)));
}

std::pair<std::string, std::string> GoProController::_webcamVersion(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/version";
    return std::pair<std::string, std::string>(target, exec(getCommand(url)));
}

std::pair<std::string, std::string> GoProController::_getMediaList(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/media/list";
    return std::pair<std::string, std::string>(target, exec(getCommand(url)));
}