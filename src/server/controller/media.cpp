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

