/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

std::string GoProController::queryStatus(std::string target){
    json arr = json::array();
    json res = json::object();
    json hw = json::object();
    std::string address;
    if(target.size() > 0){
        try{
            SingleResponse result = _queryStatus(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        try{
            SingleResponse result = _queryHW(target);
            address = result.first;
            hw = json::parse(result.second);
        }catch(const std::exception& ex){
            hw = json::object();
        }
        json i = json::object();
        i["ip"] = address;
        i["status"] = res;
        i["hw"] = hw;
        camera_hw[address] = hw;
        arr.push_back(i);
    }else{
        std::vector<std::string> buffer = std::vector<std::string>(camera_alive_ips.size());
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            std::copy(std::begin(camera_alive_ips), std::end(camera_alive_ips), std::begin(buffer));
        }
        std::vector<SingleResponse> results = _queryAllStatus(buffer);
        std::vector<SingleResponse> hresults = _queryAllHW(buffer);
        for(int32_t i = 0; i < results.size(); i++){
            try{
                address = results[i].first;
                res = json::parse(results[i].second);
                hw = json::parse(hresults[i].second);
            }catch(const std::exception& ex){
                res = json::object();
                hw = json::object();
            }
            json j = json::object();
            j["ip"] = address;
            j["status"] = res;
            j["hw"] = hw;
            camera_hw[address] = hw;
            arr.push_back(j);
        }
    }
    return arr.dump();
}

std::string GoProController::setSetting(std::string target, int32_t ID, std::string value){
    json arr = json::array();
    json res = json::object();
    std::string address = "";
    if(target.size() > 0){
        try{
            SingleResponse result = _setSetting(target, ID, value);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            address = "";
            res = json::object();
            std::cerr << "[ERROR] setSetting: " << ex.what() << std::endl;
        }
        json i;
        i["ip"] = target;
        i["status"] = res;
        arr.push_back(i);
    }else{
        std::vector<std::string> buffer = std::vector<std::string>(camera_alive_ips.size());
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            std::copy(std::begin(camera_alive_ips), std::end(camera_alive_ips), std::begin(buffer));
        }
        std::vector<SingleResponse> results = _setAllSetting(buffer, ID, value);
        for(int32_t i = 0; i < results.size(); i++){
            try{
                address = results[i].first;
                res = json::parse(results[i].second);
            }catch(const std::exception& ex){
                address = results[i].first;
                res = json::object();
                std::cerr << "[ERROR] setSetting: " << ex.what() << std::endl;
            }
            json j;
            j["ip"] = address;
            j["status"] = res;
            arr.push_back(j);
        }
    }
    return arr.dump();
}

std::string GoProController::setSettingAll(const std::string source, const std::string target, int32_t preset, json value){
    json arr = json::array();
    json res = json::object();
    std::string address = "";
    if(target.size() > 0){ // Apply to single target
        std::vector<SingleResponse> results = _setSetting(target, preset, value);
        for(int32_t i = 0; i < results.size(); i++){
            try{
                address = results[i].first;
                res = json::parse(results[i].second);
            }catch(const std::exception& ex){
                address = "";
                res = json::object();
                std::cerr << "[ERROR] setSettingAll: " << ex.what() << std::endl;
            }
            json j = json::object();
            j["ip"] = address;
            j["status"] = res;
            arr.push_back(j);
        }
    }else{ // Apply to all
        std::vector<std::string> buffer = std::vector<std::string>();
        {
            std::lock_guard<std::mutex> lock(ips_alive_mutex);
            for(int32_t i = 0; i < camera_alive_ips.size(); i++){
                if(camera_alive_ips[i] != source) buffer.push_back(camera_alive_ips[i]);
            }
        }
        std::vector<SingleResponse> results = _setAllSetting(buffer, preset, value);
        for(int32_t i = 0; i < results.size(); i++){
            try{
                address = results[i].first;
                res = json::parse(results[i].second);
            }catch(const std::exception& ex){
                address = results[i].first;
                res = json::object();
                std::cerr << "[ERROR] setSettingAll: " << ex.what() << std::endl;
            }
            json j = json::object();
            j["ip"] = address;
            j["status"] = res;
            arr.push_back(j);
        }
    }
    return arr.dump();
}

