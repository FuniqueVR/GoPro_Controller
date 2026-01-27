#include "GoProController.hpp"
#include "../common/iphelper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>   // For std::chrono::seconds, milliseconds, etc.
#include <thread>   // For std::this_thread::sleep_for
#include <future>

std::string getPacket(std::string key, json data){
    json response;
    response["key"] = key;
    response["value"] = data;
    return response.dump();
}

GoProController::GoProController() {
    _loadRecord();
}

GoProController::~GoProController() {
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
    mdns.executeDiscovery();
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


void GoProController::startRecording() {
    // Command: /gopro/camera/shutter/start
    
}

void GoProController::stopRecording() {
    // Command: /gopro/camera/shutter/stop
    
}

void GoProController::setModePhoto() {
    
}

void GoProController::setModeVideo() {
    
}

void GoProController::reboot(std::string target){
    if(target.size() > 0){
        _reboot(target);
    }else{
        for(std::string ip : camera_ips){
            _reboot(ip);
        }
    }
}

void GoProController::shutdown(std::string target){
    if(target.size() > 0){
        _shutdown(target);
    }else{
        for(std::string ip : camera_ips){
            _shutdown(ip);
        }
    }
}

void GoProController::keep_alive(std::string target){
    if(target.size() > 0){
        _keep_alive(target);
    }else{
        for(std::string ip : camera_ips){
            _keep_alive(ip);
        }
    }
}

void GoProController::usb(std::string target, bool ison){
    if(target.size() > 0){
        _usb(target, ison);
    }else{
        for(std::string ip : camera_ips){
            _usb(ip, ison);
        }
    }
}

void GoProController::datetime(std::string target){
    if(target.size() > 0){
        _datetime(target);
    }else{
        for(std::string ip : camera_ips){
            _datetime(ip);
        }
    }
}

void GoProController::zoom(std::string target){
    if(target.size() > 0){
        _zoom(target);
    }else{
        for(std::string ip : camera_ips){
            _zoom(ip);
        }
    }
}

void GoProController::shutter(std::string target, bool isstart){
    if(target.size() > 0){
        _shutter(target, isstart);
    }else{
        for(std::string ip : camera_ips){
            _shutter(ip, isstart);
        }
    }
}

std::string GoProController::queryStatus(std::string target){
    json r;
    json arr = json::array();
    if(target.size() > 0){
        json res = json::parse(_queryStatus(target));
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        for(std::string ip : camera_ips){
            json res = json::parse(_queryStatus(ip));
            json i;
            i["ip"] = ip;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    r["data"] = arr;
    return r.dump();
}

std::string GoProController::setSetting(std::string target, int ID, std::string value){
    json r;
    json arr = json::array();
    if(target.size() > 0){
        json res = json::parse(_setSetting(target, ID, value));
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        for(std::string ip : camera_ips){
            json res = json::parse(_setSetting(ip, ID, value));
            json i;
            i["ip"] = ip;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    r["data"] = arr;
    return r.dump();
}


void GoProController::webcamMode(std::string target){
    if(target.size() > 0){
        _webcamMode(target);
    }else{
        for(std::string ip : camera_ips){
            _webcamMode(ip);
        }
    }
}

void GoProController::webcamUnMode(std::string target){
    if(target.size() > 0){
        _webcamUnMode(target);
    }else{
        for(std::string ip : camera_ips){
            _webcamUnMode(ip);
        }
    }
}

void GoProController::webcamOn(std::string target, int startPort, int res, int fov, bool TS){
    if(target.size() > 0){
        _webcamOn(target, startPort, res, fov, TS);
    }else{
        for(std::string ip : camera_ips){
            _webcamOn(ip, startPort, res, fov, TS);
        }
    }
}

void GoProController::webcamOff(std::string target){
    if(target.size() > 0){
        _webcamOff(target);
    }else{
        for(std::string ip : camera_ips){
            _webcamOff(ip);
        }
    }
}

std::string GoProController::webcamStatus(std::string target){
    json r;
    json arr = json::array();
    if(target.size() > 0){
        json res = json::parse(_webcamStatus(target));
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        for(std::string ip : camera_ips){
            json res = json::parse(_webcamStatus(ip));
            json i;
            i["ip"] = ip;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    r["data"] = arr;
    return r.dump();
}

std::string GoProController::webcamVersion(std::string target){
    json r;
    json arr = json::array();
    if(target.size() > 0){
        json res = json::parse(_webcamVersion(target));
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        for(std::string ip : camera_ips){
            json res = json::parse(_webcamVersion(ip));
            json i;
            i["ip"] = ip;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    r["data"] = arr;
    return r.dump();
}

std::string GoProController::getMediaList(std::string target){
    json r;
    json arr = json::array();
    if(target.size() > 0){
        json res = json::parse(_getMediaList(target));
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        for(std::string ip : camera_ips){
            json res = json::parse(_getMediaList(ip));
            json i;
            i["ip"] = ip;
            i["status"] = res;
            arr.push_back(i);
        }
    }
    r["data"] = arr;
    return r.dump();
}


std::string GoProController::getAllIP(){
    json result = json::array();
    for(std::string target : camera_ips){
        result.push_back(target);
    }
    return result.dump();
}

void GoProController::_loadRecord(){
    std::ifstream inFile("record.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return; // Return with an error code
    }
    std::string line;
    while (std::getline(inFile, line)) {
        camera_ips.push_back(line);
    }
    inFile.close();
}

void GoProController::_updateRecord(){
    std::ofstream outFile("record.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return; // Return with an error code
    }
    for(auto i : camera_ips){
        outFile << i << "\n";
    }
    outFile.close();
}

void GoProController::_reboot(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gp/gpControl/command/system/reset";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_shutdown(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gp/gpControl/command/system/shutdown";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_keep_alive(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/keep_alive";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_usb(std::string target, bool ison){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/control/wired_usb?p=";
    if(ison) url += "1";
    else url += "0";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_datetime(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/control/wired_usb?p=0";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_zoom(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/control/wired_usb?p=0";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_shutter(std::string target, bool ison){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/shutter/";
    if(ison) url += "start";
    else url += "stop";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

std::string GoProController::_queryStatus(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/state";
    std::string command = std::string("curl -s \"" + url + "\"");
    std::string json_result = exec(command);
    return json_result;
}

std::string GoProController::_setSetting(std::string target, int ID, std::string value){
    std::string url = GetRemoteURLByIP(target) + "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    std::string command = std::string("curl -s \"" + url + "\"");
    std::string json_result = exec(command);
    return json_result;
}

void GoProController::_webcamMode(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/preview";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_webcamUnMode(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/exit";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
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
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

void GoProController::_webcamOff(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/stop";
    std::string command = std::string("curl -s \"" + url + "\"");
    exec(command);
}

std::string GoProController::_webcamStatus(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/status";
    std::string command = std::string("curl -s \"" + url + "\"");
    return exec(command);
}

std::string GoProController::_webcamVersion(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/webcam/version";
    std::string command = std::string("curl -s \"" + url + "\"");
    return exec(command);
}

std::string GoProController::_getMediaList(std::string target){
    std::string url = GetRemoteURLByIP(target) + "/gopro/media/list";
    std::string command = std::string("curl -s \"" + url + "\"");
    return exec(command);
}