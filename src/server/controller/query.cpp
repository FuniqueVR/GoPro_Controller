/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

std::string GoProController::queryStatus(std::string target){
    json arr = json::array();
    json res = json::object();
    json hw = json::object();
    std::string address;
    if(target.size() > 0){
        try{
            std::pair<std::string, std::string> result = _queryStatus(target);
            address = result.first;
            res = json::parse(result.second);
        }catch(const std::exception& ex){
            res = json::object();
        }
        try{
            std::pair<std::string, std::string> result = _queryHW(target);
            address = result.first;
            hw = json::parse(result.second);
        }catch(const std::exception& ex){
            hw = json::object();
        }
        json i = json::object();
        i["ip"] = address;
        i["status"] = res;
        i["hw"] = hw;
        arr.push_back(i);
    }else{
        std::lock_guard<std::mutex> lock(ips_mutex);
        std::vector<std::pair<std::string, std::string>> results = _queryAllStatus(camera_ips);
        std::vector<std::pair<std::string, std::string>> hresults = _queryAllHW(camera_ips);
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

