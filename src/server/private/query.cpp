/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>

std::vector<std::pair<std::string, std::string>> GoProController::_queryAllStatus(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/state");
}

std::pair<std::string, std::string> GoProController::_queryStatus(std::string target){
    return _getSingleResponse(target, "/gopro/camera/state");
}

std::vector<std::pair<std::string, std::string>> GoProController::_queryAllHW(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/info");
}

std::pair<std::string, std::string> GoProController::_queryHW(std::string target){
    return _getSingleResponse(target, "/gopro/camera/info");
}

std::vector<std::pair<std::string, std::string>> GoProController::_setAllSetting(std::vector<std::string> targets, int32_t ID, std::string value){
    std::string url = "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return _getAllResponse(targets, url);
}

std::pair<std::string, std::string> GoProController::_setSetting(std::string target, int32_t ID, std::string value){
    std::string url = "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return _getSingleResponse(target, url);
}

std::vector<std::pair<std::string, std::string>> GoProController::_setAllSetting(std::vector<std::string> targets, json res){
    int32_t model;
    json setting;
    if(res["model"].is_number()){
        model = res["model"].get<int32_t>();
    }
    if(res["setting"].is_object()){
        setting = res["setting"];
    }
}

std::vector<std::pair<std::string, std::string>> GoProController::_setSetting(std::string target, json res){
    std::vector<std::pair<std::string, std::string>> r = std::vector<std::pair<std::string, std::string>>();
    auto a = _setSetting_utility(target, res, GOPRO_VIDEO_SETTING_IDS);
    r.insert(a.end(), a.begin(), a.end());
    auto a = _setSetting_utility(target, res, GOPRO_PHOTO_SETTING_IDS);
    r.insert(a.end(), a.begin(), a.end());
    auto a = _setSetting_utility(target, res, GOPRO_VIDEO_PROTUNE_SETTING_IDS);
    r.insert(a.end(), a.begin(), a.end());
    auto a = _setSetting_utility(target, res, GOPRO_PHOTO_PROTUNE_SETTING_IDS);
    r.insert(a.end(), a.begin(), a.end());

    return r;
}

std::vector<std::pair<std::string, std::string>> _setSetting_utility(std::string target, json res, std::vector<int32_t> setting_ids){
    std::vector<std::pair<std::string, std::string>> r = std::vector<std::pair<std::string, std::string>>();
    std::string url = "/gopro/camera/setting?option=";

    int32_t model;
    json setting;
    if(res["model"].is_number()){
        model = res["model"].get<int32_t>();
    }
    if(res["setting"].is_object()){
        setting = res["setting"];
    }

    for(int32_t i = 0; i < setting_ids.size(); i++){
        int32_t id = setting_ids.at(i);
        if(!res[std::to_string(id)].is_number()) continue;

        int32_t value = res[std::to_string(id)].get<int32_t>();
        const suburl = url + std::to_string(value);
        suburl += "&setting=";
        suburl += std::to_string(id);
        r.push_back(_getSingleResponse(target, suburl));
    }

    return r;
}