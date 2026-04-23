/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>
#include <thread>
#include <future>

void GoProController::scanCameras() {
    std::lock_guard<std::mutex> lock(ips_mutex);
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
        for(std::string ip : camera_alive_ips){
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
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _rebootAll(camera_alive_ips);
    }
}

void GoProController::shutdown(std::string target){
    if(target.size() > 0) _shutdown(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _shutdownAll(camera_alive_ips);
    }
}

void GoProController::keep_alive(std::string target){
    if(target.size() > 0) _keepAlive(target); 
    else {
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _keepAliveAll(camera_alive_ips);
    }
}

void GoProController::usb(std::string target, bool ison){
    if(target.size() > 0) _usb(target, ison); 
    else {
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _usbAll(camera_alive_ips, ison);
    }
}

void GoProController::datetime(std::string target){
    if(target.size() > 0) _datetime(target);
    else {
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _datetimeAll(camera_alive_ips);
    }
}

void GoProController::zoom(std::string target, int32_t value){
    if(target.size() > 0) _zoom(target, value); 
    else {
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _zoomAll(camera_alive_ips, value);
    }
}

void GoProController::shutter(std::string target, bool isstart){
    if(target.size() > 0) _shutter(target, isstart); 
    else {
        std::lock_guard<std::mutex> lock(ips_alive_mutex);
        _shutterAll(camera_alive_ips, isstart);
    }
}
void GoProController::locate(std::string target, bool ison){
    if(target.size() > 0) _locate(target, ison); 
}

std::string GoProController::getAllIP(){
    json result = json::array();
    for(std::string target : camera_alive_ips){
        if(camera_name.count(target)){
            result.push_back(target + " " + camera_name.at(target));
        }else{
            result.push_back(target);
        }
    }
    return result.dump();
}

std::string GoProController::getAllModel(){
    return "";
}