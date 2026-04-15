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
