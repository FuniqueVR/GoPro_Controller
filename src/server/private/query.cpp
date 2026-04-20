/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#include "../GoProController.h"
#include <vector>
#include <string>
#include "../../common/camera_code.h"

#define SETTING_UTILITY_CALL(r,a,buffer,size,field) \
buffer = std::vector<int32_t>(size); \
for(int32_t i = 0; i < size; i++){ \
    buffer[i] = field[i]; \
} \
a = _setSetting_utility(target, res, buffer); \
r.insert(a.end(), a.begin(), a.end()); \

std::vector<SingleResponse> GoProController::_queryAllStatus(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/state");
}

SingleResponse GoProController::_queryStatus(std::string target){
    return _getSingleResponse(target, "/gopro/camera/state");
}

std::vector<SingleResponse> GoProController::_queryAllHW(std::vector<std::string> targets){
    return _getAllResponse(targets, "/gopro/camera/info");
}

SingleResponse GoProController::_queryHW(std::string target){
    return _getSingleResponse(target, "/gopro/camera/info");
}

std::vector<SingleResponse> GoProController::_setAllSetting(std::vector<std::string> targets, int32_t ID, std::string value){
    std::string url = "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return _getAllResponse(targets, url);
}

SingleResponse GoProController::_setSetting(std::string target, int32_t ID, std::string value){
    std::string url = "/gopro/camera/setting?option=";
    url += value;
    url += "&setting=";
    url += std::to_string(ID);
    return _getSingleResponse(target, url);
}

std::vector<SingleResponse> GoProController::_setAllSetting(std::vector<std::string> targets, json res){
    int32_t model;
    json setting;
    if(res["model"].is_number()){
        model = res["model"].get<int32_t>();
    }
    if(res["setting"].is_object()){
        setting = res["setting"];
    }
}

std::vector<SingleResponse> GoProController::_setSetting(std::string target, json res){
    std::vector<SingleResponse> r = std::vector<SingleResponse>();
    std::vector<SingleResponse> a = std::vector<SingleResponse>();
    std::vector<int32_t> buffer = std::vector<int32_t>();

    SETTING_UTILITY_CALL(r, a, buffer, GOPRO_VIDEO_SETTING_SIZE, GOPRO_VIDEO_SETTING_IDS);
    SETTING_UTILITY_CALL(r, a, buffer, GOPRO_PHOTO_SETTING_SIZE, GOPRO_PHOTO_SETTING_IDS);
    SETTING_UTILITY_CALL(r, a, buffer, GOPRO_VIDEO_PROTUNE_SETTING_SIZE, GOPRO_VIDEO_PROTUNE_SETTING_IDS);
    SETTING_UTILITY_CALL(r, a, buffer, GOPRO_PHOTO_PROTUNE_SETTING_SIZE, GOPRO_PHOTO_PROTUNE_SETTING_IDS);

    return r;
}

std::vector<SingleResponse> GoProController::_setSetting_utility(std::string target, json res, std::vector<int32_t> setting_ids){
    std::vector<SingleResponse> r = std::vector<SingleResponse>();
    std::string url = "/gopro/camera/setting?option=";

    int32_t model;
    int32_t target_model;
    json setting;
    json target_hw;
    if(camera_hw.count(target)){
        target_hw = camera_hw.at(target);
        target_model = _get_current_model(target_hw);
    }
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
        std::string suburl = url + std::to_string(value);
        suburl += "&setting=";
        suburl += std::to_string(id);
        r.push_back(_getSingleResponse(target, suburl));
    }

    return r;
}

int32_t GoProController::_get_current_model(json hwinfo){
    if(hwinfo["model_name"].is_string()){
        std::string model_name = hwinfo["model_name"].get<std::string>();
        if(model_name == "MAX 2") return MODEL_MAX2;
        else if(model_name == "HERO13 Black") return MODEL_13;
        else if(model_name == "HERO12 Black") return MODEL_12;
        else if(model_name == "HERO11 Black") return MODEL_11_BLACK;
        else if(model_name == "HERO11 Black Mini") return MODEL_11;
        else if(model_name == "HERO10 Black") return MODEL_10;
        else if(model_name == "HERO9 Black") return MODEL_9;
    }
    return 0;
}