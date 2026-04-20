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
            SingleResponse result = _getMediaList(target);
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
        std::vector<std::future<SingleResponse>> calls = 
            std::vector<std::future<SingleResponse>>();
        for(std::string ip : camera_ips){
            calls.push_back(std::async(std::launch::async, [this, ip]() {
                return _getMediaList(ip);
            }));
        }

        for(auto& call : calls){
            try{
                SingleResponse result = call.get();
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
            SingleResponse result = _getLastMedia(target);
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
        std::vector<SingleResponse> results = _getAllLastMedia(camera_ips);
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

